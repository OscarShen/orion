#include "microfacet.h"
#include <math/linalg.h>
namespace orion {
	Spectrum MicrofacetReflection::f(const Vector3f & swi, const Vector3f & swo) const
	{
		Float cosThetaO = absCosTheta(swo), cosThetaI = absCosTheta(swi);
		Vector3f wh = swi + swo;
		if (cosThetaO == 0 || cosThetaI == 0)
			return 0;
		if (wh == 0)
			return 0;
		wh = normalize(wh);
		Spectrum F = fresnel->evaluate(dot(swi, wh));
		return R * distribution->D(wh) * distribution->G(swi, swo, wh) * F / (4 * cosThetaI * cosThetaO);
	}
	Spectrum MicrofacetReflection::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rnd, Float * pdf, BxDF_TYPE * sampledType) const
	{
		if (swo.y == 0) return 0;
		Vector3f wh = distribution->sample_wh(swo, rnd);
		*swi = reflect(swo, wh);
		if (!sameHemisphere(swo, *swi)) return 0;

		*pdf = distribution->pdf(wh, swo) / (4 * dot(swo, wh));
		return f(*swi, swo);
	}
	Float MicrofacetReflection::pdf(const Vector3f & swi, const Vector3f & swo) const
	{
		if (!sameHemisphere(swi, swo)) return 0;
		Vector3f wh = normalize(swo + swi);
		return distribution->pdf(wh, swo) / (4 * dot(wh, swo));
	}
	Float GGX::D(const Vector3f & wh) const
	{
		Float cos2t = cos2Theta(wh);
		Float tan2t = sin2Theta(wh) / cos2t;
		Float root = alpha / (cos2t * (alpha * alpha + tan2t));
		return root * root * invpi;
	}
	Float GGX::G(const Vector3f & wi, const Vector3f & wo, const Vector3f & wh) const
	{
		auto SmithG1 = [&](const Vector3f &w, const Vector3f &wh)
		{
			Float tanT = std::abs(tanTheta(w));

			if (tanT == 0.0f)
				return 1.0f;

			if (dot(w, wh) * cosTheta(w) <= 0)
				return 0.0f;

			Float root = alpha * tanT;
			return 2.0f / (1.0f + std::sqrt(1.0f + root * root));
		};

		return SmithG1(wo, wh) * SmithG1(wi, wh);
	}
	Vector3f GGX::sample_wh(const Vector3f & wo, const Point2f & rand) const
	{
		const Float phi = rand.x * 2 * pi;
		const Float theta = std::acos(std::sqrt((1 - rand.y) / (1 + rand.y * (alpha * alpha - 1))));
		return Vector3f(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
	}
	Float GGX::pdf(const Vector3f & wh, const Vector3f & wo) const
	{
		return D(wh) * absCosTheta(wh);
	}
	MicrofacetTransmission::MicrofacetTransmission(const Spectrum & T, const std::shared_ptr<MicrofacetDistribution>& distribution, Float etaA, Float etaB, bool hasAtenuation)
		: BxDF(BxDF_TYPE(BxDF_TRANSMISSION | BxDF_GLOSSY)), T(T),
		distribution(distribution), fresnel(etaA, etaB), etaA(etaA), etaB(etaB), hasAtenuation(hasAtenuation)
	{
	}
	Spectrum MicrofacetTransmission::f(const Vector3f & swi, const Vector3f & swo) const
	{
		if (sameHemisphere(swo, swi)) return 0;  // transmission only

		Float cosThetaO = cosTheta(swo);
		Float cosThetaI = cosTheta(swi);
		if (cosThetaI == 0 || cosThetaO == 0) return Spectrum(0);

		// Compute $\wh$ from $\wo$ and $\wi$ for microfacet transmission
		Float eta = cosTheta(swo) > 0 ? (etaB / etaA) : (etaA / etaB);
		Vector3f wh = normalize(swo + swi * eta);
		if (wh.y < 0) wh = -wh;

		Spectrum F = fresnel.evaluate(dot(swo, wh));

		Float sqrtDenom = dot(swo, wh) + eta * dot(swi, wh);
		Float factor = hasAtenuation ? 1 / eta : 1;

		return (Spectrum(1.f) - F) * T *
			std::abs(distribution->D(wh) * distribution->G(swi, swo, wh) * eta * eta *
				absDot(swi, wh) * absDot(swo, wh) * factor * factor /
				(cosThetaI * cosThetaO * sqrtDenom * sqrtDenom));
	}
	Spectrum MicrofacetTransmission::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rnd, Float * pdf, BxDF_TYPE * sampledType) const
	{
		if (swo.y == 0) return 0.;
		Vector3f wh = distribution->sample_wh(swo, rnd);
		Float eta = cosTheta(swo) > 0 ? (etaA / etaB) : (etaB / etaA);
		if (!refract(swo, (Normal3f)wh, eta, swi)) return 0;
		*pdf = this->pdf(*swi, swo);
		return f(*swi, swo);
	}
	Float MicrofacetTransmission::pdf(const Vector3f & swi, const Vector3f & swo) const
	{
		if (sameHemisphere(swo, swi)) return 0;
		// Compute $\wh$ from $\wo$ and $\wi$ for microfacet transmission
		Float eta = cosTheta(swo) > 0 ? (etaB / etaA) : (etaA / etaB);
		Vector3f wh = normalize(swo + swi * eta);

		// Compute change of variables _dwh\_dwi_ for microfacet transmission
		Float sqrtDenom = dot(swo, wh) + eta * dot(swi, wh);
		Float dwh_dwi =
			std::abs((eta * eta * dot(swi, wh)) / (sqrtDenom * sqrtDenom));
		return distribution->pdf(wh, swo) * dwh_dwi;
	}
}
