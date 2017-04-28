#include "scene.h"
#include <tinyXML/tinyxml.h>
#include <util/strutil.h>
#include <util/transformcache.h>
#include <util/envvariable.h>
namespace orion {

	bool Scene::intersect(const Ray & ray, Intersection *isec) const
	{
		return accel->intersect(ray, isec);
	}
}
