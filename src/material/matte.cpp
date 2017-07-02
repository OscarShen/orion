#include "matte.h"
#include <core/bsdf.h>
#include <core/texture.h>
#include <texture/floattexture.h>
ORION_NAMESPACE_BEGIN

std::shared_ptr<BSDF> Matte::getBSDF(const Intersection & isec, TransportMode mode) const
{
	auto bsdf = std::make_shared<BSDF>(isec);
	Spectrum r = Kd->evaluate(isec).clamp();
	Float sig = sigma->evaluate(isec).clamp(0.0f, 90.0f).r;
	if (!r.isBlack()) {
		// if(sig == 0)
		bsdf->addBxDF(std::make_shared<BxDF>(r));
		// sigma != 0
		// else
	}
	return bsdf;
}

std::shared_ptr<Matte> createMatteMaterial(std::shared_ptr<Texture> Kd, std::shared_ptr<FloatTexture> sigma)
{
	return std::make_shared<Matte>(Kd, sigma);
}

ORION_NAMESPACE_END
