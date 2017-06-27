#include "scene.h"
#include <embree2/rtcore.h>
ORION_NAMESPACE_BEGIN
void Scene::build()
{
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
	_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
	int numTri = (int)primitives.size(), numVer = numTri * 3;
	RTCGeometryFlags geom_flags = RTC_GEOMETRY_STATIC;

	rtc_device = rtcNewDevice(NULL);

}
bool Scene::intersect(const Ray & ray, Intersection * isec) const
{
}
ORION_NAMESPACE_END
