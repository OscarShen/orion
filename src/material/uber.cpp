#include "uber.h"
#include <core/texture.h>
#include <core/bsdf.h>
#include <texture/floattexture.h>
#include <bsdf/lambert.h>
#include <bsdf/specular.h>
#include <bsdf/microfacet.h>
ORION_NAMESPACE_BEGIN
std::shared_ptr<BSDF> Uber::getBSDF(const Intersection & isec, TransportMode mode) const
{
	auto bsdf = std::make_shared<BSDF>(isec);
	Float e = eta->evaluate(isec).r;
	Spectrum op = opacity->evaluate(isec);
	Spectrum t = (Spectrum(1.0f) - op).clamp();
	if (!t.isBlack())
		bsdf->addBxDF(std::make_shared<SpecularTransmission>(t, 1.0f, 1.0f, mode));

	Spectrum kd = op * Kd->evaluate(isec).clamp();
	if (!kd.isBlack())
		bsdf->addBxDF(std::make_shared<LambertReflection>(kd));

	Spectrum ks = op * Ks->evaluate(isec).clamp();
	if (!ks.isBlack()) {
		auto fresnel = std::make_shared<FresnelDielectric>(1.0f, e);
		Float rough = roughness->evaluate(isec).r;
		Float alpha = GGX::roughnessToAlpha(rough);
		auto distrib = std::make_shared<GGX>(alpha);
		bsdf->addBxDF(std::make_shared<MicrofacetReflection>(ks, distrib, fresnel));
	}

	Spectrum kr = op * Kr->evaluate(isec).clamp();
	if (!kr.isBlack()) {
		auto fresnel = std::make_shared<FresnelDielectric>(1.0f, e);
		bsdf->addBxDF(std::make_shared<SpecularReflection>(kr, fresnel));
	}

	Spectrum kt = op * Kt->evaluate(isec).clamp();
	if (!kt.isBlack())
		bsdf->addBxDF(std::make_shared<SpecularTransmission>(kt, 1.f, e, mode));

	return bsdf;
}

std::shared_ptr<Uber> createUberMaterial(const std::shared_ptr<Texture>& Kd, const std::shared_ptr<Texture>& Ks, const std::shared_ptr<Texture>& Kr, const std::shared_ptr<Texture>& Kt, const std::shared_ptr<Texture>& opacity, const std::shared_ptr<FloatTexture>& roughness, const std::shared_ptr<FloatTexture>& eta)
{
	return std::make_shared<Uber>(Kd, Ks, Kr, Kt, opacity, roughness, eta);
}

ORION_NAMESPACE_END
