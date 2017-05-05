#include "whitted.h"
namespace orion {

	Spectrum WhittedIntegrator::Li(const Ray & ray, std::shared_ptr<Scene> scene) const
	{
		Intersection isec;
		if (!scene->intersect(ray, &isec)) {
			return Spectrum(0.34f, 0.55f, 0.85f);
		}
		Vector3f lightDir = normalize(Vector3f(0, -1.0f, -1.0f));

		Spectrum lightdensity(20.0f);

		Float density = std::max(0.0f, dot(-lightDir, isec.n));

		Spectrum t;
		auto material = isec.primitive->getMaterial();
		std::shared_ptr<BSDF> bsdf = material->getBSDF(&isec);
		Spectrum c = bsdf->f(lightDir, -ray.d);
		t = c * density * lightdensity;

		if (dot(-ray.d, isec.n) > 0.001f) {
			if (bsdf->numBxDF(BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR)) > 0)
				t += specularReflect(ray, &isec, bsdf, scene, ray.depth);
			if (bsdf->numBxDF(BxDF_TYPE(BxDF_TRANSMISSION | BxDF_SPECULAR)) > 0)
				t += specularTransmit(ray, &isec, bsdf, scene, ray.depth);
		}
		return t;
	}
	std::shared_ptr<Integrator> createWhittedIntegrator(const ParamSet & param)
	{
		return std::shared_ptr<Integrator>(new WhittedIntegrator());
	}
}
