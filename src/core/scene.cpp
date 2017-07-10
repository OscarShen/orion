#include "scene.h"
#include <embree2/rtcore.h>
#include "light.h"
ORION_NAMESPACE_BEGIN
Scene::Scene(const std::vector<std::shared_ptr<Primitive>>& primitives, const std::vector<std::shared_ptr<Light>>& lights)
	: lights(lights)
{
		// only support embree kernel now
		kernel = std::make_unique<EmbreeKernel>(primitives);
		for (const auto & light : lights) {
			light->preprocess(*this);
			if (light->type & (int)LightType::Infinite)
				envmaps.push_back(light);
		}
}
bool Scene::intersect(const Ray & ray, Intersection * isec) const
{
	return kernel->intersect(ray, isec);
}
ORION_NAMESPACE_END
