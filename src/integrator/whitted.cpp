#include "whitted.h"
namespace orion {

	Spectrum WhittedIntegrator::Li(const Ray & ray, std::shared_ptr<Scene> scene) const
	{
		Intersection isec;
		if (!scene->intersect(ray, &isec)) {
			return Spectrum();
		}
		Vector3f lightDir = normalize(Vector3f(0, -1.0f, 1.0f));

		Spectrum lightdensity(20.0f);

		Float density = std::max(0.0f, dot(-lightDir, isec.n));

		Spectrum t;
		auto material = isec.primitive->getMaterial();
		std::shared_ptr<BSDF> bsdf = material->getBSDF(&isec);
		Spectrum c = bsdf->f(lightDir, -ray.d);
		t = c * density * lightdensity;

		if (dot(-ray.d, isec.n) > 0.001f && bsdf->numBxDF(BxDF_REFLECTION) > 0)
			t += specularReflect(ray, &isec, bsdf, scene, ray.depth);
		return t;
	}
	std::shared_ptr<Integrator> createWhittedIntegrator(const ParamSet & param)
	{
		return std::shared_ptr<Integrator>(new WhittedIntegrator());
	}
}
