#include "glass.h"
#include <core/bsdf.h>
#include <core/texture.h>
#include <bsdf/specular.h>
#include <texture/floattexture.h>
#include <bsdf/specular.h>
#include <bsdf/microfacet.h>
ORION_NAMESPACE_BEGIN
std::shared_ptr<BSDF> Glass::getBSDF(const Intersection & isec, TransportMode mode) const
{
	auto bsdf = std::make_shared<BSDF>(isec);

	Float e = eta->evaluate(isec).r;
	Float rough = roughness->evaluate(isec).r;
	Spectrum R = Kr->evaluate(isec);
	Spectrum T = Kt->evaluate(isec);

	if (R.isBlack() && T.isBlack())
		return bsdf;

	bool isSpecular = rough == 0;
	Float alpha = GGX::roughnessToAlpha(rough);

	if (isSpecular) {
		bsdf->addBxDF(std::make_shared<FresnelSpecular>(R, T, 1.0f, e, mode));
	}
	else {
		std::shared_ptr<MicrofacetDistribution> distrib(new GGX(alpha));
		std::shared_ptr<Fresnel> fresnel = std::make_shared<FresnelDielectric>(1.0f, e);
		if (!R.isBlack())
			bsdf->addBxDF(std::make_shared<MicrofacetReflection>(R, distrib, fresnel));
		if (!T.isBlack())
			bsdf->addBxDF(std::make_shared<MicrofacetTransmission>(T, distrib, 1.0f, e, mode));
	}

	return bsdf;
}

std::shared_ptr<Glass> createGlassMaterial(const std::shared_ptr<Texture>& Kr, const std::shared_ptr<Texture>& Kt, const std::shared_ptr<FloatTexture>& roughness, const std::shared_ptr<FloatTexture>& eta)
{
	return std::shared_ptr<Glass>(new Glass(Kr, Kt, roughness, eta));
}

ORION_NAMESPACE_END
