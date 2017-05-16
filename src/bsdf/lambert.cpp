#include "lambert.h"

namespace orion {

	Spectrum LambertianReflection::f(const Vector3f & wi, const Vector3f & wo) const
	{
		return R * invpi;
	}

	Spectrum LambertianTransmission::f(const Vector3f & swi, const Vector3f & swo) const
	{
		return T * invpi;
	}
}
