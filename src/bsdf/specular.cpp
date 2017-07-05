#include "specular.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN
Spectrum SpecularTransmission::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rand, Float * pdf, BxDFType * sampledType) const
{
	bool entering = cosTheta(swo) > 0;
	Float etaI = entering ? etaA : etaB;
	Float etaT = entering ? etaB : etaA;

	if (!refract(swo, faceforward(Normal3f(0, 0, 1), swo), etaI / etaT, swi))
		return 0;

	*pdf = 1.0f;
	Spectrum ft = T * (Spectrum(1.0f) - fresnel.evaluate(cosTheta(*swi)));
	if (mode == TransportMode::Path)
		ft *= (etaI * etaI) / (etaT * etaT);
	return ft / absCosTheta(*swi);
}

Spectrum SpecularReflection::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rand, Float * pdf, BxDFType * sampledType) const
{
	*swi = Vector3f(-swo.x, -swo.y, swo.z);
	*pdf = 1;
	return fresnel->evaluate(cosTheta(*swi)) * R / absCosTheta(*swi);
}

ORION_NAMESPACE_END
