#include "whitted.h"
namespace orion {

	Spectrum WhittedIntegrator::Li(const Ray & ray, std::shared_ptr<Scene> scene) const
	{
		Intersection isec;
		if (!scene->intersect(ray, &isec)) {
			return Spectrum();
		}

		Vector3f lightDir = normalize(Vector3f(-1.0f, -5.0f, 7.0f));

		return dot(-ray.d, isec.n);
	}
}
