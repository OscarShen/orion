#include "merlmaterial.h"

namespace orion {
	std::shared_ptr<BSDF> MerlMaterial::getBSDF(const Intersection * isec) const
	{
		BSDF *bsdf = new BSDF(isec);
		bsdf->addBxDF(bxdf);
		return std::shared_ptr<BSDF>(bsdf);
	}
	std::shared_ptr<MerlMaterial> createMerlMaterial(const ParamSet & param)
	{
		auto merl = createMerl(param);
		return std::make_shared<MerlMaterial>(merl);
	}
}
