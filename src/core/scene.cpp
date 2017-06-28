#include "scene.h"
#include <embree2/rtcore.h>
ORION_NAMESPACE_BEGIN
bool Scene::intersect(const Ray & ray, Intersection * isec) const
{
	return kernel->intersect(ray, isec);
}
ORION_NAMESPACE_END
