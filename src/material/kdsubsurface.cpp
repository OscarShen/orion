#include "kdsubsurface.h"
#include <core/texture.h>
#include <texture/floattexture.h>
#include <core/bsdf.h>
#include <core/bssrdf.h>
#include <bsdf/specular.h>
#include <bsdf/microfacet.h>
#include <texture/constant.h>
#include <util/param.h>
#include <util/strutil.h>
ORION_NAMESPACE_BEGIN
std::shared_ptr<BSDF> KdSubsurface::getBSDF(const Intersection & isec, TransportMode mode) const
{
	Spectrum R = Kr->evaluate(isec).clamp();
	Spectrum T = Kt->evaluate(isec).clamp();
	Float rough = roughness->evaluate(isec).r;

	std::shared_ptr<BSDF> bsdf(new BSDF(isec));

	if (R.isBlack() && T.isBlack())
		return bsdf;

	if (rough == 0) {
		bsdf->addBxDF(std::make_shared<FresnelSpecular>(R, T, 1.0f, eta, mode));
	}
	else {
		Float alpha = GGX::roughnessToAlpha(rough);
		std::shared_ptr<MicrofacetDistribution> distrib(new GGX(alpha));
		std::shared_ptr<Fresnel> fresnel(new FresnelDielectric(1.0f, eta));

		if (!R.isBlack())
			bsdf->addBxDF(std::make_shared<MicrofacetReflection>(R, distrib, fresnel));
		if (!T.isBlack())
			bsdf->addBxDF(std::make_shared<MicrofacetTransmission>(T, distrib, 1.0f, eta, mode));
	}

	return bsdf;
}
std::shared_ptr<BSSRDF> KdSubsurface::getBSSRDF(const Intersection & isec, TransportMode mode) const
{
	Spectrum mfree = scale * mfp->evaluate(isec).clamp();
	Spectrum kd = Kd->evaluate(isec).clamp();
	Spectrum sig_a, sig_s;
	subsurfaceFromDiffuse(table, kd, mfree, &sig_a, &sig_s);
	return std::make_shared<TabulatedBSSRDF>(isec, eta, this, mode, sig_a, sig_s, table);
}

std::shared_ptr<KdSubsurface> createKdSubsurfaceMaterial(std::shared_ptr<Texture>& Kd, std::shared_ptr<Texture>& mfp, std::shared_ptr<Texture>& Kr, std::shared_ptr<Texture>& Kt, std::shared_ptr<FloatTexture>& roughness, const ParamSet & param)
{
	if (Kd == nullptr)
		Kd.reset(new ConstantTexture(0.5f));
	if (mfp == nullptr)
		mfp.reset(new ConstantTexture(1.0f));
	if (Kr == nullptr)
		Kr.reset(new ConstantTexture(1.0f));
	if (Kt == nullptr)
		Kt.reset(new ConstantTexture(1.0f));
	if (roughness == nullptr)
		roughness.reset(new FloatTexture(0));

	Float eta = param.hasParam("eta") ? parseFloat(param.getParam("eta")) : 1.33f;
	Float scale = param.hasParam("scale") ? parseFloat(param.getParam("scale")) : 1.0f;
	Float g = param.hasParam("g") ? parseFloat(param.getParam("g")) : 0;
	return std::shared_ptr<KdSubsurface>(new KdSubsurface(scale, Kd, Kr, Kt, mfp, g, eta, roughness));
}

ORION_NAMESPACE_END
