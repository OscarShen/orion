#include "integrator.h"

namespace orion {

	Spectrum Integrator::specularReflect(const Ray & ray, const Intersection * isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		Ray r;
		Spectrum f = bsdf->sample_f(r.d, -ray.d, 0, BxDF_REFLECTION);
		r.d = normalize(r.d);
		r.depth++;
		r.o = isec->pHit + r.d * 5e-6f; // TODO : a littel bias
		return f * Li(r, scene);
	}

	Spectrum Integrator::specularTransmit(const Ray & ray, const Intersection * isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const
	{
		ERROR("No impl!");
		return Spectrum();
	}
}
