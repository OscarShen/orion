#include "glass.h"
#include <bsdf/reflection.h>
#include <common/paramset.h>
#include <util/strutil.h>
namespace orion {

	std::shared_ptr<BSDF> GlassMaterial::getBSDF(const Intersection * isec) const
	{
		std::shared_ptr<BSDF> bsdf(new BSDF(isec));
		std::shared_ptr<Fresnel> fresnel(new FresnelDielectric(1.0f, eta));
		bsdf->addBxDF(std::make_shared<SpecularTransmission>(Spectrum(1.0f), 1.0f, eta));
		bsdf->addBxDF(std::make_shared<SpecularReflection>(fresnel));
		return bsdf;
	}
	std::shared_ptr<GlassMaterial> createGlassMaterial(const ParamSet & param)
	{
		Float eta = parseFloat(param.getParam("eta"));
		return std::shared_ptr<GlassMaterial>(new GlassMaterial(eta));
	}
}
