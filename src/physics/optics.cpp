#include "optics.h"
#include <math/linalg.h>
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
		return (Rparl * Rparl + Rperp * Rperp) / 2; // ²»¿¼ÂÇÆ«Õñ·½Ïò
	}

}
