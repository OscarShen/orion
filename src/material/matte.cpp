#include "matte.h"
#include <texture/floattexture.h>
#include <texture/constant.h>
#include <texture/checkboard.h>
#include <bsdf/lambert.h>
#include <bsdf/orennayar.h>
#include <util/texmanager.h>
#include <util/strutil.h>
namespace orion {

	std::shared_ptr<BSDF> MatteMaterial::getBSDF(const Intersection * isec) const
	{
		Spectrum color = Kd->evaluate(isec);
		Float sigma = this->sigma->evaluate(isec).r;

		auto bsdf = std::make_shared<BSDF>(isec);
		if (!color.isBlack()) {
			if (sigma == 0)
				bsdf->addBxDF(std::make_shared<LambertianReflection>(color));
			else
				bsdf->addBxDF(std::make_shared<OrenNayar>(color, sigma));
		}
		return bsdf;
	}
	std::shared_ptr<Material> createMatteMaterial(std::shared_ptr<Texture> &Kd, std::shared_ptr<Texture> &sigma)
	{
		if (Kd == nullptr) {
			Kd.reset(new ConstantTexture(Spectrum(0.5f)));
			CHECK_INFO(false, "no kd, use default!");
		}
		if (sigma == nullptr)
			sigma.reset(new FloatTexture(0));
		return std::shared_ptr<Material>(new MatteMaterial(Kd, sigma));
	}
}
