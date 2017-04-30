#include "whitted.h"
namespace orion {

	Spectrum WhittedIntegrator::Li(const Ray & ray, std::shared_ptr<Scene> scene) const
	{
		Intersection isec;
		if (!scene->intersect(ray, &isec)) {
			return Spectrum();
		}

		Vector3f lightDir = normalize(Vector3f(-1.0f, -5.0f, 7.0f));

		Spectrum t(1.0f);
		if (isec.primitive->getMaterial()) {
			std::shared_ptr<BSDF> bsdf = isec.primitive->getMaterial()->getBSDF(&isec);
			if (bsdf)
				t = bsdf->f(lightDir, -ray.d);
		}

		return t * dot(-ray.d, isec.n);
	}
	std::shared_ptr<Integrator> createWhittedIntegrator(const ParamSet & param)
	{
		return std::shared_ptr<Integrator>(new WhittedIntegrator());
	}
	std::shared_ptr<Integrator> createWhittedIntegrator(const ParamSet & param)
	{
		return std::shared_ptr<Integrator>(new WhittedIntegrator());
	}
}
