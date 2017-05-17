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
	std::shared_ptr<Material> createMatteMaterial(const ParamSet & param)
	{
		std::string type = param.getParam("type");
		bool stringEmpty = false;
		std::shared_ptr<Texture> tex;
		if (type == "image") {
			std::string filename = param.getParam("filename");
			if (!filename.empty())
				tex = TexManager::inst()->read(filename);
			else
				stringEmpty = true;
		}
		else if (type == "checkbox") {
			std::string color0Str = param.getParam("color0");
			std::string color1Str = param.getParam("color1");
			if (!color0Str.empty() && !color1Str.empty()) {
				Spectrum color0 = parseSpectrum(color0Str);
				Spectrum color1 = parseSpectrum(color1Str);
				tex.reset(new CheckBoardTexture(color0, color1));
			}
			else
				stringEmpty = true;
		}
		else if (type == "constant") {
			std::string colorStr = param.getParam("color");
			if (!colorStr.empty()) {
				Spectrum color = parseSpectrum(colorStr);
				tex.reset(new ConstantTexture(color));
			}
		}

		std::shared_ptr<FloatTexture> sigma;
		if (param.hasParam("sigma")) {
			sigma.reset(new FloatTexture(parseFloat(param.getParam("sigma"))));
		}
		else {
			sigma.reset(new FloatTexture(0));
		}


		if(stringEmpty)
			tex.reset(new ConstantTexture(0.5f));
		return std::shared_ptr<Material>(new MatteMaterial(tex, sigma));
	}
}
