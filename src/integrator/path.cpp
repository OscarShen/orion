#include "path.h"
#include <core/scene.h>
#include <core/light.h>
#include <core/primitive.h>
#include <core/material.h>
#include <core/bsdf.h>
#include <core/sampler.h>
#include <core/camera.h>
#include <sampler/sampling.h>
#include <util/param.h>
#include <util/strutil.h>
#include <util/renderinfo.h>
ORION_NAMESPACE_BEGIN

void PathTracing::render(const Scene & scene)
{
	preprocess(scene, *sampler);
	auto film = camera->getFilm();
	int filmHeight = film->getHeight(), filmWidth = film->getWidth();
	int filmSamples = filmHeight * filmWidth;
	static ProcessReporter reporter(filmHeight * filmWidth * nSamples);
	for (int k = 0; k < nSamples; ++k) {
#pragma omp parallel for
		for (int j = 0; j < filmHeight; ++j) {
			for (int i = 0; i < filmWidth; ++i) {
				std::shared_ptr<Sampler> s = sampler->clone(k * filmSamples + j * filmWidth + i);
				Ray ray = camera->generateRay(Point2f((Float)i, (Float)j), s);

				Spectrum ret = Li(ray, scene, *s);

				accumulate(i, j, ret);

				reporter.done();
			}
		}
	}
}

void PathTracing::preprocess(const Scene & scene, Sampler & sampler)
{
	int n = (int)scene.lights.size();
	CHECK_INFO(n > 0, "no light, you will get a black picture!");
	std::vector<Float> pdfs(n);
	for (int i = 0; i < n; ++i)
		pdfs[i] = scene.lights[i]->power().intensity();
	// We have only one light distribution which is power distribution.
	lightDistrib = std::shared_ptr<Distribution1D>(new Distribution1D(pdfs.data(), n));
}

Spectrum PathTracing::Li(const Ray & a, const Scene & scene, Sampler & sampler, int depth) const
{
	Spectrum L, beta(1.0f);
	Ray ray = a;
	bool specularBounce = false;
	int bounces;

	for (bounces = 0;; ++bounces) {
		Intersection isec;
		bool foundIntersection = scene.intersect(ray, &isec);

		if (bounces == 0 || specularBounce) {
			if (foundIntersection) {
				L += beta * isec.Le(-ray.d);
			}
			else {
				// infinity area light
				return 0;
			}
		}

		if (!foundIntersection || bounces >= maxDepth)
			break;

		isec.calculateBSDF();

		if (isec.bsdf->numBxDF(BxDFType(BxDF_ALL & ~BxDF_SPECULAR)) > 0) {
			Spectrum Ld = beta * uniformSampleOneLight(ray, isec, scene, sampler, lightDistrib.get());
			L += Ld;
		}

		Vector3f wo = -ray.d, wi;
		Float pdf;
		BxDFType type;
		Spectrum f = isec.bsdf->sample_f(&wi, wo, sampler.next2(), &pdf, BxDF_ALL, &type);
		if (f.isBlack() || pdf == 0)
			break;
		beta *= f * absDot(wi, isec.ns) / pdf;
		specularBounce = (type & BxDF_SPECULAR) != 0;

		ray = isec.spawnRay(wi);
		if (bounces > 3) {
			Float q = std::max((Float)0.05, 1 - beta.maxComponentValue());
			if (sampler.next() < q)
				break;
			beta /= 1 - q;
		}
	}
	return L;
}

void PathTracing::accumulate(int i, int j, const Spectrum & s)
{
	outputLock.lock();
	camera->getFilm()->acumulate(i, j, s);
	outputLock.unlock();
}

std::shared_ptr<PathTracing> createPathTracingIntegrator(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Sampler>& sampler, const ParamSet & param)
{
	int maxDepth = parseInt(param.getParam("maxDepth"));
	int nSamples = parseInt(param.getParam("nSamplesPerPixel"));
	return std::make_shared<PathTracing>(camera, sampler, maxDepth, nSamples);
}

ORION_NAMESPACE_END
