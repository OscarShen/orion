#include "lambert.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN

Spectrum LambertReflection::f(const Vector3f & swi, const Vector3f & swo) const
{
	return R * invpi;
}


Spectrum LambertTransmission::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rand, Float * pdf, BxDFType * sampledType) const
{
	*swi = cosineSampleHemisphere(rand);
	if (swo.z > 0)
		swi->z *= -1;
	*pdf = this->pdf(*swi, swo);
	return f(*swi, swo);
}

Spectrum LambertTransmission::f(const Vector3f & swi, const Vector3f & swo) const
{
	return T * invpi;
}

Float LambertTransmission::pdf(const Vector3f & swi, const Vector3f & swo) const
{
	return sameHemisphere(swi, swo) ? 0 : absCosTheta(swi) * invpi;
}

ORION_NAMESPACE_END
