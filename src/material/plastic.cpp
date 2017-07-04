#include "plastic.h"
#include <texture/texture.h>
#include <texture/floattexture.h>
#include <bsdf/lambert.h>
#include <bsdf/microfacet.h>
#include <physics/optics.h>
namespace orion{
	std::shared_ptr<BSDF> PlasticMaterial::getBSDF(const Intersection * isec, bool hasEtaAtenuation) const
	{
		std::shared_ptr<BSDF> bsdf(new BSDF(isec));
		Spectrum kd = Kd->evaluate(isec).clamp();
		if (!kd.isBlack())
			bsdf->addBxDF(std::shared_ptr<BxDF>(new LambertianReflection(kd)));
		Spectrum ks = Ks->evaluate(isec).clamp();
		if (!ks.isBlack()) {
			std::shared_ptr<Fresnel> fresnel(new FresnelDielectric(1.5f, 1.0f));
			Float rough = roughness->evaluate(isec).r;
			std::shared_ptr<MicrofacetDistribution> distrib(new GGX(rough));
			std::shared_ptr<MicrofacetReflection> refl(new MicrofacetReflection(ks, distrib, fresnel));
			bsdf->addBxDF(refl);
		}
		return bsdf;
	}
	std::shared_ptr<PlasticMaterial> createPlasticMaterial( std::shared_ptr<Texture>& Kd, std::shared_ptr<Texture>& Ks, std::shared_ptr<Texture>& roughness)
	{
		return std::shared_ptr<PlasticMaterial>(new PlasticMaterial(Kd, Ks, roughness));
	}
}
