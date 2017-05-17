#include "orennayar.h"
#include <math/linalg.h>
namespace orion {

	OrenNayar::OrenNayar(const Spectrum & R, Float sigma)
		: BxDF(BxDF_TYPE(BxDF_REFLECTION | BxDF_DIFFUSE)), R(R)
	{
		sigma = radians(sigma);
		Float sigma2 = sigma * sigma;
		A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
		B = 0.45f * sigma2 / (sigma2 + 0.09f);
	}
	Spectrum OrenNayar::f(const Vector3f & swi, const Vector3f & swo) const
	{
		Float sinthetaI = sinTheta(swi);
		Float sinthetaO = sinTheta(swo);
		Float maxCos = 0;
		if (sinthetaI > 1e-4 && sinthetaO > 1e-4) {
			Float sinPhiI = sinPhi(swi), cosPhiI = cosPhi(swi);
			Float sinPhiO = sinPhi(swo), cosPhiO = cosPhi(swo);
			Float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
			maxCos = std::max((Float)0, dCos);
		}
		Float sinAlpha, tanBeta;
		if (absCosTheta(swi) > absCosTheta(swo)) {
			sinAlpha = sinthetaO;
			tanBeta = sinthetaI / absCosTheta(swi);
		}
		else {
			sinAlpha = sinthetaI;
			tanBeta = sinthetaO / absCosTheta(swo);
		}
		return R * invpi * (A + B * maxCos * sinAlpha * tanBeta);
	}
}
