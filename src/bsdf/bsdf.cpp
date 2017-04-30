#include "bsdf.h"

namespace orion {

	Spectrum BSDF::f(const Vector3f & wi, const Vector3f & wo) const
	{
		Spectrum r;
		for (size_t i = 0; i < bxdf.size(); ++i) {
			r += bxdf[i]->f(wi, wo);
		}
		return r;
	}
}
