#include "refraction.h"

namespace orion {

	Spectrum Refraction::f(const Vector3f & wi, const Vector3f & wo) const
	{
		ERROR("no imp");
		return Spectrum();
	}

	Spectrum Refraction::sample_f(Vector3f & wi, const Vector3f & wo, Float * pdf) const
	{
		ERROR("no imp");
		return Spectrum();
	}
}
