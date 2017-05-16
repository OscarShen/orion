#include "intersection.h"
#include "primitive.h"
#include <light/arealight.h>
namespace orion {

	Spectrum Intersection::Le(const Vector3f & wo) const
	{
		auto area = primitive->getAreaLight();
		return area ? area->L(*this, wo) : 0;
	}
}
