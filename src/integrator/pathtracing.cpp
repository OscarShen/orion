#include "pathtracing.h"
#include <sampler/sampler.h>
#include <light/light.h>
#include <util/strutil.h>
namespace orion {
	void PathTracing::preprocess(const Scene & scene, Sampler & sampler)
	{
		int n = (int)scene.lights.size();
		CHECK_INFO(n > 0, "no light, you will get a black picture!");
		std::vector<Float> pdfs(n);
		for (int i = 0; i < n; ++i)
			pdfs[i] = scene.lights[i]->power().intensity();
		// We have only one light distribution which is power distribution.
		lightDistrib.reset(new Distribution1D(pdfs.data(), n));
	}
	Spectrum PathTracing::Li(const Ray & ray, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Sampler>& sampler, int depth) const
	{
		Spectrum L(0);

		bool specularBounce = true;
		Spectrum pathWeight(1.0f);
		int bounces = 0;
		Ray r = ray;
		while (true) {
			Intersection isec;

			bool foundInterseection = scene->intersect(r, &isec);
			if (bounces == 0 || specularBounce) {
				if (foundInterseection) {
					L += pathWeight * isec.Le(-ray.d);
				}
				else {
					//L += envSpectrum;
				}
			}
			if (!foundInterseection || bounces >= maxDepth) break;

			// direct light
			auto bsdf = isec.primitive->getMaterial()->getBSDF(&isec);
			Float lightPdf = 0;
			Point2f lightSample = sampler->next2(), bsdfSample = sampler->next2();
			// L += Ld
			L += pathWeight * uniformSampleOneLight(ray, isec, *scene, *sampler, lightDistrib);

			Vector3f wo = -ray.d, wi;
			Float pdf;
			BxDF_TYPE flags;
			Spectrum f = bsdf->sample_f(&wi, wo, sampler->next2(), &pdf, BxDF_ALL, &flags);

			if (f.isBlack() || pdf == 0) break;
			pathWeight *= f * absDot(wi, isec.n) / pdf;

			if (pathWeight.intensity() == 0)
				break;
			if(bounces > 3)

			specularBounce = (flags & BxDF_SPECULAR) != 0;

			r = isec.spawnRay(wi);

			// TODO : Add bssrdf

			if (bounces > 3 && !specularBounce) {
				Float q = std::max(0.05f, 1 - pathWeight.maxComponentValue());
				if (sampler->next() < q)
					break;
				pathWeight /= 1 - q;
			}
			++bounces;
		}
		return L;
	}
	std::shared_ptr<PathTracing> createPathTracingIntegrator(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Sampler>& sampler, const ParamSet & param)
	{
		int maxDepth = parseInt(param.getParam("maxDepth"));
		return std::shared_ptr<PathTracing>(new PathTracing(camera, sampler, maxDepth));
	}
}
