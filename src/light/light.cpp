#include "light.h"
#include <core/intersection.h>
#include <math/linalg.h>
#include <common/paramset.h>
#include <util/strutil.h>
namespace orion {

	Spectrum PointLight::sample_Li(const Intersection & isec, Vector3f * wi, Float * pdf) const
	{
		Vector3f v = p - isec.pHit;
		*wi = normalize(v);
		*pdf = 1.0f;
		return I / v.lengthSquared();
	}
	std::shared_ptr<PointLight> createPointLight(const Transform & light2world, const ParamSet & param)
	{
		Spectrum I = parseSpectrum(param.getParam("intensity"));
		return std::shared_ptr<PointLight>(new PointLight(light2world, I));
	}
	std::shared_ptr<SpotLight> createSpotLight(const Transform & light2world, const ParamSet & param)
	{
		Spectrum I = parseSpectrum(param.getParam("intensity"));
		Float coneangle = parseFloat(param.getParam("angle"));
		Float conedelta = parseFloat(param.getParam("delta"));
		return std::shared_ptr<SpotLight>(new SpotLight(light2world, I, coneangle, coneangle - conedelta));
	}
	SpotLight::SpotLight(const Transform & light2world, const Spectrum & I, Float totalWidth, Float falloffStart)
		: Light(light2world), I(I), p(light2world(Point3f(0))),
		cosTotalWidth(std::cos(radians(totalWidth))),
		cosFalloffStart(std::cos(radians(cosFalloffStart))) {}
	Spectrum SpotLight::sample_Li(const Intersection & isec, Vector3f * wi, Float * pdf) const
	{
		Vector3f v = p - isec.pHit;
		*wi = normalize(v);
		*pdf = 1.0f;
		return I * falloff(-*wi) / v.lengthSquared();
	}
	Float SpotLight::falloff(const Vector3f & w) const
	{
		Vector3f wl = normalize(world2local(w));
		Float cosTh = cosTheta(wl);
		if (cosTh < cosTotalWidth) return 0;
		if (cosTh > cosFalloffStart) return 1;
		// compute falloff inside spotlight cone
		Float delta = (cosTh - cosTotalWidth) / (cosFalloffStart - cosTotalWidth);
		return delta * delta * delta * delta;
	}
}
