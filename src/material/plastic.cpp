#include "plastic.h"
#include <core/texture.h>
#include <texture/floattexture.h>
#include <core/bsdf.h>
#include <bsdf/lambert.h>
#include <bsdf/microfacet.h>
ORION_NAMESPACE_BEGIN
std::shared_ptr<BSDF> Plastic::getBSDF(const Intersection & isec, TransportMode mode) const
{
	auto bsdf = std::make_shared<BSDF>(isec);
	Spectrum kd = Kd->evaluate(isec).clamp();
	if (!kd.isBlack())
		bsdf->addBxDF(std::make_shared<LambertReflection>(kd));

	Spectrum ks = Ks->evaluate(isec).clamp();
	if (!ks.isBlack()) {
		auto fresnel = std::make_shared<FresnelDielectric>(1.5f, 1.0f); // a fixed figure
		Float rough = roughness->evaluate(isec).r;
		Float alpha = GGX::roughnessToAlpha(rough);
		auto distrib = std::make_shared<GGX>(alpha);
		bsdf->addBxDF(std::make_shared<MicrofacetReflection>(ks, distrib, fresnel));
	}
	return bsdf;
}

std::shared_ptr<Plastic> createPlasticMaterial(const std::shared_ptr<Texture>& Kd, const std::shared_ptr<Texture>& Ks, const std::shared_ptr<FloatTexture>& roughness)
{
	return std::make_shared<Plastic>(Kd, Ks, roughness);
}

ORION_NAMESPACE_END
