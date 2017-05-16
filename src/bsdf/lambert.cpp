#include "lambert.h"

namespace orion {

	Spectrum Lambert::f(const Vector3f & wi, const Vector3f & wo) const
	{
		return s * invpi;
	}

	Spectrum Lambert::sample_f(Vector3f * wi, const Vector3f & wo, Float * pdf) const
	{
		ERROR("no imp");
		return Spectrum();
	}

}
