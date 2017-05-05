#include "whitted.h"
#include <light/light.h>
namespace orion {

	Spectrum WhittedIntegrator::Li(const Ray & ray, std::shared_ptr<Scene> scene) const
	{
		if (ray.depth > 5)
			return 0.0f;
		Intersection isec;
		if (!scene->intersect(ray, &isec)) {
			return Spectrum(0.34f, 0.55f, 0.85f);
		}

		Spectrum t(0.0f);
		auto material = isec.primitive->getMaterial();
		std::shared_ptr<BSDF> bsdf = material->getBSDF(&isec);

		for (auto it = scene->lights.begin(); it != scene->lights.end(); ++it) {
			Vector3f lightDir;
			Float pdf;
			Spectrum I = (*it)->sample_Li(isec, &lightDir, &pdf);
			Spectrum f = bsdf->f(-ray.d, lightDir);
			t += I * f * std::max(0.0f, dot(isec.n, lightDir)) / pdf;
		}

		if (bsdf->numBxDF(BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR)) > 0)
			t += specularReflect(ray, &isec, bsdf, scene, ray.depth);
		if (bsdf->numBxDF(BxDF_TYPE(BxDF_TRANSMISSION | BxDF_SPECULAR)) > 0)
			t += specularTransmit(ray, &isec, bsdf, scene, ray.depth);
		return t;
	}
	std::shared_ptr<Integrator> createWhittedIntegrator(const ParamSet & param)
	{
		return std::shared_ptr<Integrator>(new WhittedIntegrator());
	}
}
