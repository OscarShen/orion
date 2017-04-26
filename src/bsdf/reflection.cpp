#include "reflection.h"

namespace orion {

	Spectrum Reflection::f(const Vector3f & wi, const Vector3f & wo) const
	{
		return Spectrum();
	}
	Spectrum Reflection::sample_f(Vector3f & wi, const Vector3f & wo, Float * pdf) const
	{
		return Spectrum();
	}
}
