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
		if (hasAtenuation)
			ft *= (etaI * etaI) / (etaT * etaT);
		return ft / absCosTheta(*swi);
	}
	Spectrum FresnelSpecular::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rnd, Float * pdf, BxDF_TYPE * sampledType) const
	{
		Float F = frDielectric(cosTheta(swo), etaA, etaB);
		if (rnd[0] < F) {
			*swi = Vector3f(-swo.x, swo.y, -swo.z);
			if (sampledType)
				*sampledType = BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR);
			*pdf = F;
			return F * R / absCosTheta(*swi);
		}
		else {
			bool entering = cosTheta(swo) > 0;
			Float etaI = entering ? etaA : etaB;
			Float etaT = entering ? etaB : etaA;

			if (!refract(swo, faceforward(Normal3f(0, 1, 0), swo), etaI / etaT, swi))
				return 0;
			Spectrum ft = T * (1 - F);

			if (hasAtenuation)
				ft *= (etaI * etaI) / (etaT * etaT);
			if (sampledType)
				*sampledType = BxDF_TYPE(BxDF_SPECULAR | BxDF_TRANSMISSION);
			*pdf = 1 - F;
			return ft / absCosTheta(*swi);
		}
	}
}
