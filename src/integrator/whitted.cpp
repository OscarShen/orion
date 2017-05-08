#include "whitted.h"
#include <light/light.h>
namespace orion {

	Spectrum WhittedIntegrator::Li(const Ray & ray, std::shared_ptr<Scene> scene) const
	{
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
			ShadowTester sdt;
			Spectrum Li = (*it)->sample_Li(isec, &lightDir, &pdf, &sdt);
			if (Li.isBlack() || pdf == 0) continue;
			Spectrum f = bsdf->f(-ray.d, lightDir);
			if (!f.isBlack() && sdt.unoccluded(scene))
				t += Li * f * std::max(0.0f, dot(isec.n, lightDir)) / pdf;
		}

		if (ray.depth + 1 < 5) {
			t += specularReflect(ray, &isec, bsdf, scene, ray.depth);
			t += specularTransmit(ray, &isec, bsdf, scene, ray.depth);
		}
		return t;
	}
	std::shared_ptr<Integrator> createWhittedIntegrator(const ParamSet & param)
	{
		return std::shared_ptr<Integrator>(new WhittedIntegrator());
	}
}
