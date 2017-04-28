#include "primitive.h"

namespace orion {

	bool Primitive::intersect(const Ray & ray, Intersection * isec) const
	{
		if (!shape->intersect(ray, isec))
			return false;
		isec->primitive = this;
		return true;
	}

}
