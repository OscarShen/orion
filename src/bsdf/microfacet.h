/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_MICROFACET_H_
#define ORION_BSDF_MICROFACET_H_

#include <core/bsdf.h>
ORION_NAMESPACE_BEGIN

class MicrofacetDistribution
{
public:
	virtual ~MicrofacetDistribution() {}
	virtual Float D(const Vector3f &wh) const = 0;
	virtual Float lambda(const Vector3f &w) const = 0;
	Float G(const Vector3f &wi, const Vector3f &wo) const {
		return 1 / (1 + lambda(wi) + lambda(wo));
	}
	virtual Vector3f sample_wh(const Vector3f &wo, const Point2f &rand) const = 0;
	Float pdf(const Vector3f &wh) const;
};

class GGX : public MicrofacetDistribution
{
private:
	Float alpha;

public:
	// remap roughness to get more plausible effect which roughness in [0, 1]
	static inline Float roughnessToAlpha(Float roughness);
	GGX(Float alpha) : alpha(alpha) {}
	Float D(const Vector3f &wh) const override;
	Vector3f sample_wh(const Vector3f &wo, const Point2f &rand) const override;

private:
	Float lambda(const Vector3f &w) const override;
};

inline Float GGX::roughnessToAlpha(Float roughness)
{
	roughness = std::max(roughness, (Float)1e-3);
	Float x = std::log(roughness);
	return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
		0.000640711f * x * x * x * x;
}

class MicrofacetReflection : public BxDF
{
private:
	Spectrum R;
	std::shared_ptr<MicrofacetDistribution> distrib;
	std::shared_ptr<Fresnel> fresnel;

public:
	MicrofacetReflection(const Spectrum &R,
		const std::shared_ptr<MicrofacetDistribution> &distrib,
		const std::shared_ptr<Fresnel> &fresnel)
		:BxDF(BxDFType(BxDF_REFLECTION | BxDF_GLOSSY)), R(R),
		distrib(distrib), fresnel(fresnel) {}

	Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rand,
		Float *pdf, BxDFType *sampledType = nullptr) const override;
	Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;
	Float pdf(const Vector3f &swi, const Vector3f &swo) const override;
};

class MicrofacetTransmission : public BxDF
{
private:
	Spectrum T;
	std::shared_ptr<MicrofacetDistribution> distrib;
	Float etaA, etaB;
	FresnelDielectric fresnel;
	TransportMode mode;

public:
	MicrofacetTransmission(const Spectrum &T,
		const std::shared_ptr<MicrofacetDistribution> &distrib, Float etaA,
		Float etaB, TransportMode mode)
		: BxDF(BxDFType(BxDF_TRANSMISSION | BxDF_GLOSSY)),
		T(T),
		distrib(distrib),
		etaA(etaA),
		etaB(etaB),
		fresnel(etaA, etaB),
		mode(mode) {}

	Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rand,
		Float *pdf, BxDFType *sampledType = nullptr) const override;
	Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;
	Float pdf(const Vector3f &swi, const Vector3f &swo) const override;
};

ORION_NAMESPACE_END
#endif // !ORION_BSDF_MICROFACET_H_
