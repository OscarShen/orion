#include "mirror.h"
#include <bsdf/reflection.h>
#include <common/paramset.h>
#include <util/texmanager.h>
#include <util/strutil.h>
#include <texture/checkboard.h>
#include <texture/constant.h>
namespace orion {

	std::shared_ptr<BSDF> Mirror::getBSDF(const Intersection * isec) const
	{
		std::shared_ptr<BSDF> bsdf(new BSDF(isec));
		std::shared_ptr<Fresnel> fresnel(new FresnelNoOp());
		bsdf->addBxDF(std::shared_ptr<SpecularReflection>(new SpecularReflection(fresnel)));
		return bsdf;
	}
	std::shared_ptr<Mirror> createMirrorMaterial(const ParamSet & param)
	{
		return std::shared_ptr<Mirror>(new Mirror());
	}
}
