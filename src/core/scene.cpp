#include "scene.h"
#include <sampler/sampling.h>
namespace orion {
	Scene::Scene(std::shared_ptr<Accelerator> accel, const std::vector<std::shared_ptr<Light>>& lights)
		: accel(accel), lights(lights), bound(accel->worldBound())
	{
	}
	bool Scene::intersect(const Ray & ray, Intersection *isec) const
	{
		return accel->intersect(ray, isec);
	}
}
