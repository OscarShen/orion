#include "intersection.h"
#include "light.h"
ORION_NAMESPACE_BEGIN
Spectrum Intersection::Le(const Vector3f & wo) const
{
	const AreaLight *area = primitive->getAreaLight();
	return area ? area->L(*this, wo) : 0;
}
ORION_NAMESPACE_END
