#include "integrator.h"

namespace orion {

	Spectrum Integrator::specularReflect(const Ray & ray, const Intersection * isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Ray r;
		Float pdf;
		r.depth = ray.depth;
		Spectrum f = bsdf->sample_f(r.d, -ray.d, &pdf, BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR));
		r.d = normalize(r.d);
		r.depth++;
		r.o = isec->pHit + r.d * 5e-6f; // TODO : a littel bias
		return f * Li(r, scene) / pdf;
	}

	Spectrum Integrator::specularTransmit(const Ray & ray, const Intersection * isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Ray r;
		Float pdf;
		Spectrum f = bsdf->sample_f(r.d, -ray.d, &pdf, BxDF_TYPE(BxDF_TRANSMISSION | BxDF_SPECULAR));
		r.d = normalize(r.d);
		r.depth++;
		r.o = isec->pHit + r.d * 5e-6f; // TODO : a littel bias
		return f * Li(r, scene) / pdf;
	}
}
