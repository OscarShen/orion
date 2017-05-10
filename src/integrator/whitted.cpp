#include "whitted.h"
#include <light/light.h>
#include <util/strutil.h>
#include <sampler/sampler.h>
namespace orion {

	Spectrum WhittedIntegrator::Li(const Ray &ray, const std::shared_ptr<Scene> &scene, const std::shared_ptr<Sampler> &sampler, int depth) const
	{
		Intersection isec;
		if (!scene->intersect(ray, &isec)) {
			return Spectrum(0.34f, 0.55f, 0.85f);
		}

		Spectrum t(0.0f);
		auto material = isec.primitive->getMaterial();
		std::shared_ptr<BSDF> bsdf = material->getBSDF(&isec);

		for (const auto &light : scene->lights) {
			Vector3f lightDir;
			Float pdf;
			ShadowTester sdt;
			Spectrum Li = light->sample_Li(isec, sampler->next2(), &lightDir, &pdf, &sdt);
			if (Li.isBlack() || pdf == 0) continue;
			Spectrum f = bsdf->f(-ray.d, lightDir);
			if (!f.isBlack() && sdt.unoccluded(scene))
				t += Li * f * std::max(0.0f, dot(isec.n, lightDir)) / pdf;
		}

		if (depth + 1 < maxDepth) {
			t += specularReflect(ray, &isec, sampler, bsdf, scene, depth);
			t += specularTransmit(ray, &isec, sampler, bsdf, scene, depth);
		}
		return t;
	}
	std::shared_ptr<Integrator> createWhittedIntegrator(const std::shared_ptr<const Camera> &camera, 
		const std::shared_ptr<Sampler> &sampler, const ParamSet & param)
	{
		int maxDepth = parseInt(param.getParam("maxDepth"));
		return std::shared_ptr<Integrator>(new WhittedIntegrator(camera, sampler, maxDepth));
	}
}
