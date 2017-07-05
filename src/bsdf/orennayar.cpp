#include "orennayar.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN

OrenNayar::OrenNayar(const Spectrum & R, Float sigma)
	: BxDF(BxDFType(BxDF_REFLECTION | BxDF_DIFFUSE)), R(R) {
	sigma = radians(sigma);
	Float sigma2 = sigma * sigma;
	A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
	B = 0.45f * sigma2 / (sigma2 + 0.09f);
}
Spectrum OrenNayar::f(const Vector3f & swi, const Vector3f & swo) const
{
	Float sinThetaI = sinTheta(swi);
	Float sinThetaO = sinTheta(swo);

	// cosine term
	Float cosTerm = 0;
	if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
		Float sinPhiI = sinPhi(swi), cosPhiI = cosPhi(swi);
		Float sinPhiO = sinPhi(swo), cosPhiO = cosPhi(swo);
		Float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
		cosTerm = std::max((Float)0, dCos);
	}

	// sine term & tangent term
	Float sinAlpha, tanBeta;
	if (absCosTheta(swi) > absCosTheta(swo)) {
		sinAlpha = sinThetaO;
		tanBeta = sinThetaI / absCosTheta(swi);
	}
	else {
		sinAlpha = sinThetaI;
		tanBeta = sinThetaO / absCosTheta(swo);
	}
	return R * invpi * (A + B * cosTerm * sinAlpha * tanBeta);
}
ORION_NAMESPACE_END
