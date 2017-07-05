#include "bsdf.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN
int BSDF::numBxDF(BxDFType type) const
{
	int num = 0;
	for (const auto &b : bxdfs) {
		if (b->matchType(type))
			++num;
	}
	return num;
}

constexpr Float oneMinusEpsilon = (Float)0.99999994;

Spectrum BSDF::f(const Vector3f & wi, const Vector3f & wo, BxDFType type) const
{
	Spectrum r;

	Vector3f swi = world2local(wi); // just rotation, need not to normalize
	Vector3f swo = world2local(wo);

	for (size_t i = 0; i < bxdfs.size(); ++i) {
		if (bxdfs[i]->matchType(type))
			r += bxdfs[i]->f(swi, swo);
	}
	return r;
}

Spectrum BSDF::sample_f(Vector3f * wi, const Vector3f & wo, const Point2f & rand, Float * pdf, BxDFType type, BxDFType * sampledType) const
{
	int matchingBxdfs = numBxDF(type);
	if (matchingBxdfs == 0) {
		*pdf = 0;
		if (sampledType)
			*sampledType = BxDFType(0);
		return 0;
	}

	// select a bxdf randomly
	int pos = std::min((int)std::floor(rand[0] * matchingBxdfs), matchingBxdfs - 1);
	int count = pos;
	std::shared_ptr<BxDF> bxdf;
	for (size_t i = 0; i < bxdfs.size(); ++i)
		if (bxdfs[i]->matchType(type) && count-- == 0) {
			bxdf = bxdfs[i];
			break;
		}

	Point2f newRand(std::min(rand[0] * matchingBxdfs - pos, oneMinusEpsilon), rand[1]);
	Vector3f swi, swo = world2local(wo);
	if (swo.z == 0)
		return 0;
	*pdf = 0;
	if (sampledType)
		*sampledType = bxdf->type;
	Spectrum f = bxdf->sample_f(&swi, swo, newRand, pdf, sampledType);
	if (*pdf == 0) {
		if (sampledType)
			*sampledType = BxDFType(0);
		return 0;
	}
	*wi = local2world(swi);

	bool hasSpecular = (bxdf->type & BxDF_SPECULAR) != 0;

	if (!hasSpecular && matchingBxdfs > 1)
		for (size_t i = 0; i < bxdfs.size(); ++i)
			if (bxdfs[i] != bxdf && bxdfs[i]->matchType(type))
				*pdf += bxdfs[i]->pdf(swi, swo);
	if (matchingBxdfs > 1) *pdf /= matchingBxdfs;

	if (!hasSpecular && matchingBxdfs > 1) {
		bool reflect = dot(*wi, ng) * dot(wo, ng) > 0;
		f = 0; // We will compute the _f_ again
		for (size_t i = 0; i < bxdfs.size(); ++i) {
			bool hasReflect = (bxdfs[i]->type & BxDF_REFLECTION) != 0;
			bool hasTransmission = (bxdfs[i]->type & BxDF_TRANSMISSION) != 0;
			if (bxdfs[i]->matchType(type) &&
				((reflect && hasReflect) ||
				(!reflect && hasTransmission))) {
				f += bxdfs[i]->f(swi, swo);
			}
		}
	}
	return f;
}

Float BSDF::pdf(const Vector3f & wi, const Vector3f & wo, BxDFType type) const
{
	if (bxdfs.size() == 0)
		return 0;
	Vector3f swi = world2local(wi), swo = world2local(wo);
	if (swo.z == 0)
		return 0;
	Float pdf = 0;
	int matchingBxdfs = 0;
	for (const auto &b : bxdfs) {
		if (b->matchType(type)) {
			++matchingBxdfs;
			pdf += b->pdf(swi, swo);
		}
	}
	Float v = matchingBxdfs > 0 ? pdf / matchingBxdfs : 0.f;
	return v;
}


Spectrum BxDF::sample_f(Vector3f * swi, const Vector3f & swo, const Point2f & rand, Float * pdf, BxDFType * sampledType) const
{
	*swi = cosineSampleHemisphere(rand);
	if (swo.z < 0)
		swi->z *= -1;
	*pdf = this->pdf(*swi, swo);
	return f(*swi, swo);
}

Float BxDF::pdf(const Vector3f & swi, const Vector3f & swo) const
{
	return sameHemisphere(swi, swo) ? absCosTheta(swi) * invpi : 0;
}

// https://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations/
Spectrum FrConductor(Float cosThetaI, const Spectrum &etai,
	const Spectrum &etat, const Spectrum &k) {
	cosThetaI = clamp(cosThetaI, -1.0f, 1.0f);
	Spectrum eta = etat / etai;
	Spectrum etak = k / etai;

	Float cosThetaI2 = cosThetaI * cosThetaI;
	Float sinThetaI2 = Float(1.) - cosThetaI2;
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

	return Float(0.5) * (Rp + Rs);
}

Float FrDielectric(Float cosThetaI, Float etaI, Float etaT) {
	cosThetaI = clamp(cosThetaI, (Float)(-1.0f), (Float)(1.0f));
	// Potentially swap indices of refraction
	bool entering = cosThetaI > 0.f;
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
	return (Rparl * Rparl + Rperp * Rperp) / 2;
}

Spectrum FresnelConductor::evaluate(Float cosThetaI) const
{
	return FrConductor(std::abs(cosThetaI), etaI, etaT, k);
}

Spectrum FresnelDielectric::evaluate(Float cosThetaI) const
{
	return FrDielectric(std::abs(cosThetaI), etaI, etaT);
}

ORION_NAMESPACE_END
