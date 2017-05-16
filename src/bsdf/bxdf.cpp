#include "bxdf.h"
#include <math/linalg.h>
namespace orion {

	Float BxDF::pdf(const Vector3f & wi, const Vector3f & wo) const
	{
		return sameHemisphere(wi, wo) ? absCosTheta(wi) * invpi : 0;
	}
}
