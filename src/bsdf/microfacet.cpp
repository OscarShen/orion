#include "microfacet.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN
Float MicrofacetDistribution::pdf(const Vector3f & wh) const
{
	return D(wh) * absCosTheta(wh);
}

Float GGX::D(const Vector3f & wh) const
{
	Float tan2Th = tan2Theta(wh);
	if (std::isinf(tan2Th))
		return 0;
	Float cos4Th = cos2Theta(wh) * cos2Theta(wh);
	Float alpha2 = alpha * alpha;
	Float e = tan2Th / alpha2;
	return 1 / (pi * alpha2 * cos4Th * (1 + e) * (1 + e));
}

Float GGX::lambda(const Vector3f & w) const
{
	Float absTanTheta = std::abs(tanTheta(w));
	if (std::isinf(absTanTheta)) return 0.;
	Float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
	return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
}

Vector3f GGX::sample_wh(const Vector3f & wo, const Point2f & rand) const
{
	Float tanTheta2 = alpha * alpha * rand[0] / (1.0f - rand[0]);
	Float cosTheta = 1 / std::sqrt(1 + tanTheta2);
	Float sinTheta = std::sqrt(std::max((Float)0., (Float)1. - cosTheta * cosTheta));

	Float phi = (2 * pi) * rand[1];

	Vector3f wh = Vector3f(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
	if (!sameHemisphere(wo, wh))
		wh = -wh;
	return wh;
}

Spectrum MicrofacetReflection::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rand, Float * pdf, BxDFType * sampledType) const
{
	if (swo.z == 0)
		return 0;
	Vector3f wh = distrib->sample_wh(swo, rand);
	*swi = reflect(swo, wh);
	if (!sameHemisphere(*swi, swo))
		return 0;
	*pdf = distrib->pdf(wh) / (4 * dot(swo, wh));
	return f(*swi, swo);
}

Spectrum MicrofacetReflection::f(const Vector3f & swi, const Vector3f & swo) const
{
	Float cosThetaO = absCosTheta(swo), cosThetaI = absCosTheta(swi);
	if (cosThetaI == 0 || cosThetaO == 0)
		return 0;
	Vector3f wh = swi + swo;
	if (wh.x == 0 && wh.y == 0 && wh.z == 0)
		return 0;
	wh = normalize(wh);
	Spectrum F = fresnel->evaluate(dot(swi, wh));
	return R * distrib->D(wh) * distrib->G(swi, swi) * F /
		(4 * cosThetaI * cosThetaO);
}

Float MicrofacetReflection::pdf(const Vector3f & swi, const Vector3f & swo) const
{
	if (!sameHemisphere(swi, swo))
		return 0;
	Vector3f wh = normalize(swi + swo);
	return distrib->pdf(wh) / (4 * dot(swo, wh));
}

Spectrum MicrofacetTransmission::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rand, Float * pdf, BxDFType * sampledType) const
{
	if (swo.z == 0)
		return 0;
	Vector3f wh = distrib->sample_wh(swo, rand);
	Float eta = cosTheta(swo) > 0 ? (etaA / etaB) : (etaB / etaA);
	if (!refract(swo, (Normal3f)wh, eta, swi))
		return 0;
	return f(*swi, swo);
}

Spectrum MicrofacetTransmission::f(const Vector3f & swi, const Vector3f & swo) const
{
	if (sameHemisphere(swi, swo))
		return 0;
	Float cosThetaO = cosTheta(swo);
	Float cosThetaI = cosTheta(swi);
	if (cosThetaI == 0 || cosThetaO == 0)
		return 0;
	Float eta = cosTheta(swo) > 0 ? (etaB / etaA) : (etaA / etaB);
	Vector3f wh = normalize(swo + swi * eta);
	if (wh.z < 0) wh = -wh;

	Spectrum F = fresnel.evaluate(dot(swo, wh));

	Float sqrtDenom = dot(swo, wh) + eta * dot(swi, wh);
	Float factor = (mode == TransportMode::Path) ? (1 / eta) : 1;

	return (Spectrum(1.f) - F) * T *
		std::abs(distrib->D(wh) * distrib->G(swi, swo) * eta * eta *
			absDot(swi, wh) * absDot(swo, wh) * factor * factor /
			(cosThetaI * cosThetaO * sqrtDenom * sqrtDenom));
}

Float MicrofacetTransmission::pdf(const Vector3f & swi, const Vector3f & swo) const
{
	if (sameHemisphere(swo, swi)) return 0;
	Float eta = cosTheta(swo) > 0 ? (etaB / etaA) : (etaA / etaB);
	Vector3f wh = normalize(swo + swi * eta);

	// Compute change of variables _dwh\_dwi_ for microfacet transmission
	Float sqrtDenom = dot(swo, wh) + eta * dot(swi, wh);
	Float dwh_dwi = std::abs((eta * eta * dot(swi, wh)) / (sqrtDenom * sqrtDenom));
	return distrib->pdf(wh) * dwh_dwi;
}

ORION_NAMESPACE_END
