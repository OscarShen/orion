#include "mirror.h"
#include <core/texture.h>
#include <core/bsdf.h>
#include <bsdf/specular.h>
ORION_NAMESPACE_BEGIN
std::shared_ptr<BSDF> Mirror::getBSDF(const Intersection & isec, TransportMode mode) const
{
	auto bsdf = std::make_shared<BSDF>(isec);
	Spectrum R = Kr->evaluate(isec).clamp();
	if (!R.isBlack())
		bsdf->addBxDF(std::make_shared<SpecularReflection>(R, std::make_shared<FresnelNoOp>()));
	return bsdf;
}

std::shared_ptr<Mirror> createMirrorMaterial(const std::shared_ptr<Texture>& Kr)
{
	return std::make_shared<Mirror>(Kr);
}

ORION_NAMESPACE_END
