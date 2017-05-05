#include "integrator.h"

namespace orion {

	Spectrum Integrator::specularReflect(const Ray & ray, const Intersection * isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Ray r;
		r.depth = ray.depth;
		Spectrum f = bsdf->sample_f(r.d, -ray.d, 0, BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR));
		r.d = normalize(r.d);
		r.depth++;
		r.o = isec->pHit + r.d * 5e-6f; // TODO : a littel bias
		if(depth < 3)
			return f * Li(r, scene);
		return Spectrum(0);
	}

	Spectrum Integrator::specularTransmit(const Ray & ray, const Intersection * isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Ray r;
		Spectrum f = bsdf->sample_f(r.d, -ray.d, 0, BxDF_TYPE(BxDF_TRANSMISSION | BxDF_SPECULAR));
		r.d = normalize(r.d);
		r.depth++;
		r.o = isec->pHit + r.d * 5e-6f; // TODO : a littel bias
		if(depth < 3)
			return f * Li(r, scene);
		return Spectrum(0);
	}
}
