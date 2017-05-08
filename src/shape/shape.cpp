#include "shape.h"

namespace orion {

	Bounds3f Shape::worldBound() const
	{
		return (*local2world)(localBound());
	}
}
