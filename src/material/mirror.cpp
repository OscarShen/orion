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
		bsdf->addBxDF(std::shared_ptr<Reflection>(new Reflection()));
		return bsdf;
	}
	std::shared_ptr<Mirror> createMirrorMaterial(const ParamSet & param)
	{
		return std::shared_ptr<Mirror>(new Mirror());
	}
}
