#include "whitted.h"
namespace orion {

	Spectrum WhittedIntegrator::Li(const Ray & ray, std::shared_ptr<Scene> scene) const
	{
		Intersection isec;
		if (!scene->intersect(ray, &isec)) {
			return Spectrum();
		}

		Vector3f lightDir = normalize(Vector3f(15.0f, 15.0f, -15.0f));

		Spectrum t(1.0f);
		Spectrum lightdensity(30.0f);

		Float density = std::max(0.0f, dot(lightDir, isec.n));
		if (density == 0.0f)
			return Spectrum();

		if (isec.primitive->getMaterial()) {
			std::shared_ptr<BSDF> bsdf = isec.primitive->getMaterial()->getBSDF(&isec);
			if (bsdf)
				t = bsdf->f(lightDir, -ray.d);
		}

		return t * density * lightdensity;
	}
	std::shared_ptr<Integrator> createWhittedIntegrator(const ParamSet & param)
	{
		return std::shared_ptr<Integrator>(new WhittedIntegrator());
	}
}
