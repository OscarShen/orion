#include "glass.h"
#include <core/bsdf.h>
#include <core/texture.h>
#include <bsdf/specular.h>
#include <texture/floattexture.h>
ORION_NAMESPACE_BEGIN
std::shared_ptr<BSDF> Glass::getBSDF(const Intersection & isec, TransportMode mode) const
{
	auto bsdf = std::make_shared<BSDF>(isec);

	Spectrum T = Kt->evaluate(isec);
	Float e = eta->evaluate(isec).r;
	if (/*R.isBlack() && */ T.isBlack())
		return bsdf;

	bsdf->addBxDF(std::make_shared<SpecularTransmission>(T, 1.0f, e, mode));

	return bsdf;
}

std::shared_ptr<Glass> createGlassMaterial(const std::shared_ptr<Texture>& Kt, const std::shared_ptr<FloatTexture>& eta)
{
	return std::make_shared<Glass>(Kt, eta);
}

ORION_NAMESPACE_END
