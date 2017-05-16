#include "specular.h"
#include <physics/optics.h>
#include "bsdf.h"
namespace orion {

	Spectrum SpecularReflection::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rnd, Float * pdf, BxDF_TYPE * sampledType) const
	{
		*swi = Vector3f(-swo.x, swo.y, -swo.z);
		*pdf = 1.0f;
		return fresnel->evaluate(cosTheta(*swi)) * R / absCosTheta(*swi);
	}
	Spectrum SpecularTransmission::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rnd, Float * pdf, BxDF_TYPE * sampledType) const
	{
		bool entering = cosTheta(swo) > 0;
		// incident eta
		Float etaI = entering ? etaA : etaB;
		Float etaT = entering ? etaB : etaA;

		// compute ray direction
		if (!refract(swo, faceforward(Normal3f(0, 1, 0), swo), etaI / etaT, swi)) {
			return 0;
		}
		*pdf = 1.0f;
		Spectrum ft = T * (Spectrum(1.0f) - fresnel.evaluate(cosTheta(*swi)));
		// Conservation of energy
		ft *= (etaI * etaI) / (etaT * etaT);
		return ft / absCosTheta(*swi);
	}
}
