#include "mirror.h"
#include <bsdf/specular.h>
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
		bsdf->addBxDF(std::shared_ptr<SpecularReflection>(new SpecularReflection(Spectrum(1.0f), fresnel))); // TODO : Add Ks in mirror
		return bsdf;
	}
	std::shared_ptr<Mirror> createMirrorMaterial(const ParamSet & param)
	{
		return std::shared_ptr<Mirror>(new Mirror());
	}
}
