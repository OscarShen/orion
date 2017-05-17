#include "arealight.h"
#include <shape/shape.h>
#include <util/strutil.h>
namespace orion {

	AreaLight::AreaLight(const Transform & light2world, const Spectrum & Le, int nSamples, const std::shared_ptr<Shape>& shape)
		: Light((int)LightType::Area, light2world,nSamples), area(shape->area()), Lemit(Le), shape(shape)
	{
	}
	Spectrum AreaLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi, Float * pdf, ShadowTester * sdt) const
	{
		Intersection in = shape->sample(isec, rnd, pdf);
		if (*pdf == 0 || (in.pHit - isec.pHit).lengthSquared() == 0) {
			*pdf = 0;
			return 0;
		}
		*wi = normalize(in.pHit - isec.pHit);
		*sdt = ShadowTester(isec, in);
		return L(in, -*wi);
	}
	Float AreaLight::pdf_Li(const Intersection & isec, const Vector3f & wi) const
	{
		return shape->pdf(isec, wi);
	}
	Spectrum AreaLight::L(const Intersection & isec, const Vector3f & w) const
	{
		return dot(isec.n, w) > 0 ? Lemit : 0;
	}
	std::shared_ptr<AreaLight> createAreaLight(const Transform & light2world, const std::shared_ptr<Shape>& shape, const ParamSet & param)
	{
		Spectrum Le = parseSpectrum(param.getParam("Le"));
		int nLightSamples = parseInt(param.getParam("nLightSamples"));
		return std::shared_ptr<AreaLight>(new AreaLight(light2world, Le, nLightSamples, shape));
	}
}
