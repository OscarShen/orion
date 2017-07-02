#include "integrator.h"
#include "scene.h"
#include "sampler.h"
#include "light.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN

Spectrum uniformSampleOneLight(const Ray & ray, const Intersection & isec, const Scene & scene, Sampler & sampler, const Distribution1D * lightDistrib)
{
	int nLights = (int)scene.lights.size();
	if (nLights == 0) return 0;
	int lightNum;
	Float lightPdf;
	if (lightDistrib) {
		lightNum = lightDistrib->sampleDiscrete(sampler.next(), &lightPdf);
		if (lightPdf == 0) return 0;
	}
	else {
		lightNum = std::min((int)(sampler.next() * nLights), nLights - 1);
		lightPdf = 1.0f / nLights;
	}
	const std::shared_ptr<Light> &light = scene.lights[lightNum];
	Point2f lightSample = sampler.next2();
	Point2f bsdfSample = sampler.next2();
	return estimateDirect(ray, isec, bsdfSample, *light, lightSample, scene, sampler,
		BxDFType(BxDF_ALL & ~BxDF_SPECULAR)) / lightPdf;
}

Spectrum estimateDirect(const Ray & ray, const Intersection & isec, const Point2f & BSDFSample, const Light & light, const Point2f & lightSample, const Scene & scene, Sampler & sampler, BxDFType type)
{
	auto bsdf = isec.bsdf;

	Spectrum Ld;
	Vector3f wi;
	Vector3f wo = normalize(-ray.d);
	Float lightPdf, bsdfPdf;
	ShadowTester sdt;
	Spectrum Li = light.sample_Li(isec, lightSample, &wi, &lightPdf, &sdt);
	if (lightPdf > 0 && !Li.isBlack()) {
		Spectrum f = bsdf->f(wi, wo, type) * absDot(wi, isec.ns);
		bsdfPdf = bsdf->pdf(wi, wo, type);
		if (!f.isBlack()) {
			if (!sdt.unoccluded(scene))
				Li = 0;

			if (!Li.isBlack()) {
				if (isDeltaLight(light.type))
					Ld += f * Li / lightPdf;
				else {
					Float weight = powerHeuristic(1, lightPdf, 1, bsdfPdf);
					Ld += f * Li * weight / lightPdf;
				}
			}
		}
	}

	if (!isDeltaLight(light.type)) {
		Spectrum f;
		bool sampledSpecular = false;
		BxDFType sampledType;
		f = bsdf->sample_f(&wi, wo, BSDFSample, &bsdfPdf, type, &sampledType);
		f *= absDot(wi, isec.ns);
		sampledSpecular = (sampledType & BxDF_SPECULAR) != 0;

		if (!f.isBlack() && bsdfPdf > 0) {
			Float weight = 1;
			if (!sampledSpecular) {
				lightPdf = light.pdf_Li(isec, wi, scene);
				if (lightPdf == 0)
					return Ld;
				weight = powerHeuristic(1, bsdfPdf, 1, lightPdf);
			}

			Intersection in;
			Ray rr = isec.spawnRay(wi);
			Spectrum Li;
			if (scene.intersect(rr, &in)) {
				if ((Light*)in.primitive->getAreaLight() == &light) {
					Li = in.Le(-wi);
				}
			}
			else {
				// infinite light
			}
			if (!Li.isBlack())
				Ld += f * Li * weight / bsdfPdf;
		}
	}
	return Ld;
}

ORION_NAMESPACE_END
