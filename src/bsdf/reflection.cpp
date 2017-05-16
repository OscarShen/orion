#include "reflection.h"
#include <physics/optics.h>
#include "bsdf.h"
namespace orion {

	Spectrum SpecularReflection::sample_f(Vector3f * wi, const Vector3f & wo, Float * pdf) const
	{
		*wi = Vector3f(-wo.x, wo.y, -wo.z);
		*pdf = 1.0f;
		return fresnel->evaluate(cosTheta(*wi)) / absCosTheta(*wi);
	}

	Spectrum FresnelDielectric::evaluate(Float cosThetaI) const
	{
		return frDielectric(cosThetaI, etaI, etaT);
	}
	Spectrum SpecularTransmission::sample_f(Vector3f * wi, const Vector3f & wo, Float * pdf) const
	{
		bool entering = cosTheta(wo) > 0;
		// incident eta
		Float etaI = entering ? etaA : etaB;
		Float etaT = entering ? etaB : etaA;

		*pdf = 1.0f;
		// compute ray direction
		if (!refract(wo, faceforward(Normal3f(0, 1, 0), wo), etaI / etaT, wi)) {
			return 0;
		}
		Spectrum ft = T * (Spectrum(1.0f) - fresnel.evaluate(cosTheta(*wi)));
		// Conservation of energy
		ft *= (etaI * etaI) / (etaT * etaT);
		return ft / absCosTheta(*wi);
	}
	Spectrum FresnelConductor::evaluate(Float cosThetaI) const
	{
		return frConductor(std::abs(cosThetaI), etaI, etaT, k);
	}
}
