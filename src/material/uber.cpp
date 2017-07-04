#include "uber.h"
#include <texture/texture.h>
#include <texture/floattexture.h>
#include <core/primitive.h>
#include <bsdf/specular.h>
#include <bsdf/lambert.h>
#include <bsdf/microfacet.h>
#include <texture/constant.h>
namespace orion {

	std::shared_ptr<BSDF> UberMaterial::getBSDF(const Intersection * isec, bool hasEtaAtenuation) const
	{
		Float e = eta->evaluate(isec).r;

		Spectrum op = opacity->evaluate(isec).clamp();
		Spectrum t = (-op + Spectrum(1.f)).clamp();
		std::shared_ptr<BSDF> bsdf(new BSDF(isec));
		if (!t.isBlack())
			bsdf->addBxDF(std::shared_ptr<SpecularTransmission>(new SpecularTransmission(t, 1.0f, 1.0f, hasEtaAtenuation)));

		Spectrum kd = op * Kd->evaluate(isec).clamp();
		if (!kd.isBlack())
			bsdf->addBxDF(std::shared_ptr<LambertianReflection>(new LambertianReflection(kd)));

		Spectrum ks = op * Ks->evaluate(isec).clamp();
		if (!ks.isBlack()) {
			std::shared_ptr<Fresnel> fresnel(new FresnelDielectric(1.0f, e));
			Float rough = roughness->evaluate(isec).r;
			std::shared_ptr<MicrofacetDistribution> distrib(new GGX(rough));
			bsdf->addBxDF(std::shared_ptr<MicrofacetReflection>(new MicrofacetReflection(ks, distrib, fresnel)));
		}

		Spectrum kr = op * Kr->evaluate(isec).clamp();
		if (!kr.isBlack()) {
			std::shared_ptr<Fresnel> fresnel(new FresnelDielectric(1.0f, e));
			bsdf->addBxDF(std::shared_ptr<SpecularReflection>(new SpecularReflection(kr, fresnel)));
		}

		Spectrum kt = op * Kt->evaluate(isec).clamp();
		if (!kt.isBlack()) {
			bsdf->addBxDF(std::shared_ptr<SpecularTransmission>(new SpecularTransmission(kt, 1.0f, e, hasEtaAtenuation)));
		}
		return bsdf;
	}
	std::shared_ptr<UberMaterial> createUberMaterial(const std::shared_ptr<Texture>& Kd, const std::shared_ptr<Texture>& Ks, const std::shared_ptr<Texture>& Kr, const std::shared_ptr<Texture>& Kt, const std::shared_ptr<Texture>& opacity, const std::shared_ptr<FloatTexture>& roughness, const std::shared_ptr<FloatTexture>& eta)
	{
		auto kd = Kd, ks = Ks, kr = Kr, kt = Kt, op = opacity;
		auto rough = roughness, e = eta;
		if (!kd) {
			CHECK_INFO(false, "use default Kd in uber material");
			kd.reset(new ConstantTexture(0.25f));
		}
		if (!ks) {
			CHECK_INFO(false, "use default Ks in uber material");
			ks.reset(new ConstantTexture(0.25f));
		}
		if (!kt) {
			CHECK_INFO(false, "use default Kt in uber material");
			kt.reset(new ConstantTexture(0));
		}
		if (!kr) {
			CHECK_INFO(false, "use default Kr in uber material");
			kr.reset(new ConstantTexture(0));
		}
		if (!op) {
			CHECK_INFO(false, "use default opacity in uber material");
			op.reset(new FloatTexture(1.0f));
		}
		if (!rough) {
			CHECK_INFO(false, "use default roughness in uber material");
			rough.reset(new FloatTexture(0.1f));
		}
		if (!e) {
			CHECK_INFO(false, "use default eta in uber material");
			e.reset(new FloatTexture(1.5f));
		}
		return std::shared_ptr<UberMaterial>(new UberMaterial(kd, ks, kr, kt, op, rough, e));
	}
}
