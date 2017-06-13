#include "path.h"
#include <sampler/sampler.h>
#include <light/light.h>
#include <util/strutil.h>
namespace orion {
	void PathTracing::preprocess(const Scene & scene, Sampler & sampler)
	{
		int n = (int)scene.lights.size();
		CHECK_INFO(n > 0, "no light, you will get a black picture!");
		std::vector<Float> pdfs(n);
		for (int i = 0; i < n; ++i)
			pdfs[i] = scene.lights[i]->power().intensity();
		// We have only one light distribution which is power distribution.
		lightDistrib.reset(new Distribution1D(pdfs.data(), n));
	}
	Spectrum PathTracing::Li(const Ray & a, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Sampler>& sampler, int depth) const
	{
		Spectrum L, beta(1.0f);
		Ray ray = a;
		bool specularBounce = false;
		int bounces;

		for (bounces = 0;; ++bounces) {
			Intersection isec;
			bool foundIntersection = scene->intersect(ray, &isec);
			if (bounces == 0 || specularBounce) {
				if (foundIntersection)
					L += beta *isec.Le(-ray.d);
				else
					// infinity area light
					return 0;
			}
			if (!foundIntersection || bounces >= maxDepth)
				break;
			auto bsdf = isec.primitive->getMaterial()->getBSDF(&isec);
			if (bsdf->numComponents(BxDF_TYPE(BxDF_ALL & ~BxDF_SPECULAR)) > 0) {
				Spectrum Ld = beta * uniformSampleOneLight(ray, isec, *scene, *sampler, lightDistrib);
				L += Ld;
			}

			Vector3f wo = -ray.d, wi;
			Float pdf;
			BxDF_TYPE flags;
			Spectrum f = bsdf->sample_f(&wi, wo, sampler->next2(), &pdf, BxDF_ALL, &flags);
			if (f.isBlack() || pdf == 0)
				break;
			beta *= f * absDot(wi, isec.n) / pdf;
			specularBounce = (flags & BxDF_SPECULAR) != 0;

			ray = isec.spawnRay(wi);

			// TODO : bssrdf

			if (bounces > 3) {
				Float q = std::max((Float)0.05, 1 - beta.maxComponentValue());
				if (sampler->next() < q)
					break;
				beta /= 1 - q;
			}
		}
		return L;
	}
	std::shared_ptr<PathTracing> createPathTracingIntegrator(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Sampler>& sampler, const ParamSet & param)
	{
		int maxDepth = parseInt(param.getParam("maxDepth"));
		return std::shared_ptr<PathTracing>(new PathTracing(camera, sampler, maxDepth));
	}
}
