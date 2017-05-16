#include "bxdf.h"
#include <math/linalg.h>
#include <sampler/sampling.h>
namespace orion {
	Spectrum BxDF::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rnd, Float * pdf, BxDF_TYPE * sampledType) const
	{
		*swi = cosineSampleHemisphere(rnd);
		if (swo.y < 0) // almost never occur
			swi->y *= -1;
		*pdf = BxDF::pdf(*swi, swo);
		return f(*swi, swo);
	}
	Float BxDF::pdf(const Vector3f & swi, const Vector3f & swo) const
	{
		return sameHemisphere(swi, swo) ? absCosTheta(swi) * invpi : 0;
	}
}
