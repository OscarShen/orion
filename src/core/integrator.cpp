#include "integrator.h"

namespace orion {

	Spectrum Integrator::specularReflect(const Ray & ray, const Intersection * isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Ray r;
		Float pdf;
		r.depth = ray.depth;
		Spectrum f = bsdf->sample_f(r.d, -ray.d, &pdf, BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR));
		if (!f.isBlack() && pdf > 0 && dot(r.d, isec->n) != 0) {
			r.d = normalize(r.d);
			r.depth++;
			r.o = isec->pHit + r.d * epsilon; // TODO : a littel bias
			return f * Li(r, scene) / pdf;
		}
		return Spectrum(0);
	}

	Spectrum Integrator::specularTransmit(const Ray & ray, const Intersection * isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Ray r;
		Float pdf;
		Spectrum f = bsdf->sample_f(r.d, -ray.d, &pdf, BxDF_TYPE(BxDF_TRANSMISSION | BxDF_SPECULAR));
		if (!f.isBlack() && pdf > 0 && dot(r.d, isec->n) != 0) {
			r.d = normalize(r.d);
			r.depth++;
			r.o = isec->pHit + r.d * epsilon; // TODO : a littel bias
			return f * Li(r, scene) / pdf;
		}
		return Spectrum(0);
	}
}
