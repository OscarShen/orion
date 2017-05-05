#include "optics.h"
#include <math/linalg.h>
#include <core/spectrum.h>
namespace orion {

	Float frDielectric(Float cosThetaI, Float etaI, Float etaT)
	{
		cosThetaI = clamp(cosThetaI, -1.0f, 1.0f);
		bool entering = cosThetaI > 0.0f;
		if (!entering) {
			std::swap(etaI, etaT);
			cosThetaI = std::abs(cosThetaI);
		}

		// Compute _cosThetaT_ using Snell's law
		Float sinThetaI = std::sqrt(std::max((Float)0, 1 - cosThetaI * cosThetaI));
		Float sinThetaT = etaI / etaT * sinThetaI;

		// Handle total internal reflection
		if (sinThetaT >= 1) return 1;
		Float cosThetaT = std::sqrt(std::max((Float)0, 1 - sinThetaT * sinThetaT));
		Float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
			((etaT * cosThetaI) + (etaI * cosThetaT));
		Float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
			((etaI * cosThetaI) + (etaT * cosThetaT));
		return (Rparl * Rparl + Rperp * Rperp) * 0.5f; // ²»¿¼ÂÇÆ«Õñ·½Ïò
	}

	Spectrum frConductor(Float cosThetaI, const Spectrum & etai, const Spectrum & etat, const Spectrum & k)
	{
		cosThetaI = clamp(cosThetaI, -1.0f, 1.0f);
		Spectrum eta = etat / etai;
		Spectrum etak = k / etai;

		Float cosThetaI2 = cosThetaI * cosThetaI;
		Float sinThetaI2 = 1.f - cosThetaI2;
		Spectrum eta2 = eta * eta;
		Spectrum etak2 = etak * etak;

		Spectrum t0 = eta2 - etak2 - sinThetaI2;
		Spectrum a2plusb2 = sqrt(t0 * t0 + 4 * eta2 * etak2);
		Spectrum t1 = a2plusb2 + cosThetaI2;
		Spectrum a = sqrt(0.5f * (a2plusb2 + t0));
		Spectrum t2 = (Float)2 * cosThetaI * a;
		Spectrum Rs = (t1 - t2) / (t1 + t2);

		Spectrum t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
		Spectrum t4 = t2 * sinThetaI2;
		Spectrum Rp = Rs * (t3 - t4) / (t3 + t4);

		return 0.5f * (Rp + Rs);
	}

}
