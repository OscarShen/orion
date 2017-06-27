#include "scene.h"
#include <embree2/rtcore.h>
ORION_NAMESPACE_BEGIN
void Scene::build()
{

}
bool Scene::intersect(const Ray & ray, Intersection * isec) const
{
	return false;
}
ORION_NAMESPACE_END
