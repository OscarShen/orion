#include "glass.h"
#include <bsdf/specular.h>
#include <common/paramset.h>
#include <util/strutil.h>
#include <texture/floattexture.h>
#include <bsdf/specular.h>
#include <bsdf/microfacet.h>
namespace orion {

	std::shared_ptr<BSDF> GlassMaterial::getBSDF(const Intersection * isec, bool hasEtaAtenuation) const
	{
		Float eta = this->eta->evaluate(isec).r;
		Float roughness = this->roughness->evaluate(isec).r;
		Spectrum R = Kr->evaluate(isec).clamp();
		Spectrum T = Kt->evaluate(isec).clamp();
		std::shared_ptr<BSDF> bsdf = std::make_shared<BSDF>(isec);
		if (R.isBlack() && T.isBlack())
			return bsdf;

		bool isSpecular = roughness == 0;
		if (isSpecular)
			bsdf->addBxDF(std::make_shared<FresnelSpecular>(R, T, 1.0f, eta, hasEtaAtenuation));
		else {
			auto distrib = std::make_shared<GGX>(roughness);
			if (!R.isBlack()) {
				auto fresnel = std::make_shared<FresnelDielectric>(1.0f, eta);
				bsdf->addBxDF(std::make_shared<MicrofacetReflection>(R, distrib, fresnel));
			}

			if (!T.isBlack()) {
				bsdf->addBxDF(std::make_shared<MicrofacetTransmission>(T, distrib, 1.0f, eta, hasEtaAtenuation));
			}
		}
		return bsdf;
	}
	std::shared_ptr<GlassMaterial> createGlassMaterial(const std::shared_ptr<Texture> &Kr,
		const std::shared_ptr<Texture> &Kt,
		const std::shared_ptr<FloatTexture> roughness,
		const std::shared_ptr<FloatTexture> eta)
	{
		return std::make_shared<GlassMaterial>(Kr, Kt, roughness, eta);
	}
}
