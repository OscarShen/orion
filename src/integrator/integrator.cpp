#include "integrator.h"
#include <sampler/sampler.h>
#include <sampler/samplemethod.h>
#include <light/light.h>
namespace orion {

	Spectrum Integrator::specularReflect(const Ray &ray, const Intersection *isec, const std::shared_ptr<Sampler> &sampler, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Vector3f wo = -ray.d, wi;
		Float pdf;
		BxDF_TYPE type = BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR);
		Point2f rnd = sampler->next2();
		Spectrum f = bsdf->sample_f(&wi, wo, rnd, &pdf, type);

		const Normal3f &ns = isec->n; // TODO : note shading normal
		if (pdf > 0 && !f.isBlack() && absDot(wi, ns) != 0) {
			Ray r = isec->spawnRay(wi);
			return f * Li(r, scene, sampler, depth + 1) * absDot(wi, ns) / pdf;
		}
		else
			return 0;
	}

	Spectrum Integrator::specularTransmit(const Ray &ray, const Intersection *isec, const std::shared_ptr<Sampler> &sampler, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Ray r;
		Float pdf;
		Point2f rnd = sampler->next2();
		Spectrum f = bsdf->sample_f(&r.d, -ray.d, rnd, &pdf, BxDF_TYPE(BxDF_TRANSMISSION | BxDF_SPECULAR));
		if (!f.isBlack() && pdf > 0 && dot(r.d, isec->n) != 0) {
			r.d = normalize(r.d);
			r.o = isec->pHit + r.d * epsilon;
			return f * Li(r, scene, sampler, depth + 1) / pdf;
		}
		return Spectrum(0);
	}
	Spectrum uniformSampleAllLights(const Ray &ray, const Intersection & isec, const Scene & scene, Sampler & sampler, int nSamples)
	{
		Spectrum L;
		for (size_t i = 0; i < scene.lights.size(); ++i) {
			const std::shared_ptr<Light> &light = scene.lights[i];
			Spectrum Ld;
			for (int j = 0; j < nSamples; ++j) {
				Point2f lightSample = sampler.next2(), BSDFSample = sampler.next2();
				Ld += estimateDirect(ray, isec, BSDFSample, *light, lightSample, scene, sampler, BxDF_TYPE(BxDF_ALL & ~BxDF_SPECULAR));
			}
			L += Ld / nSamples;
		}
		return L;
	}
	Spectrum estimateDirect(const Ray &ray, const Intersection & isec, const Point2f & BSDFSample, const Light & light, const Point2f & lightSample, const Scene & scene, Sampler & sampler, BxDF_TYPE type)
	{
		auto bsdf = isec.primitive->getMaterial()->getBSDF(&isec);

		Spectrum Ld;
		Vector3f wi;
		Vector3f wo = normalize(-ray.d);
		Float lightPdf, bsdfPdf;
		ShadowTester sdt;
		Spectrum Li = light.sample_Li(isec, lightSample, &wi, &lightPdf, &sdt);
		if (lightPdf > 0 && !Li.isBlack()) {
			Spectrum f = bsdf->f(wi, wo, type) * absDot(wi, isec.n);
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
			bool sampedSpecular = false;
			BxDF_TYPE sampledType;
			f = bsdf->sample_f(&wi, wo, BSDFSample, &bsdfPdf, type, &sampledType);
			f *= absDot(wi, isec.n); // TODO : add shading n
			sampedSpecular = (sampledType & BxDF_SPECULAR) != 0;

			if (!f.isBlack() && bsdfPdf > 0) {
				Float weight = 1;
				if (!sampedSpecular) {
					lightPdf = light.pdf_Li(isec, wi);
					if (lightPdf == 0) return Ld;
					weight = powerHeuristic(1, bsdfPdf, 1, lightPdf);
				}

				Intersection in;
				Ray r = isec.spawnRay(wi);
				Spectrum Li;
				if (scene.intersect(r, &in)) {
					if ((Light*)in.primitive->getAreaLight() == &light) {
						Li = in.Le(-wi);
					}
				}
				else {
					Li = Spectrum(0.34f, 0.55f, 0.85f);
				}
				if (!Li.isBlack())
					Ld += f * Li * weight / bsdfPdf;
			}
		}
		return Ld;
	}
}
