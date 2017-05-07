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
	std::shared_ptr<PointLight> createPointLight(const ParamSet & param)
	{
		Spectrum I = parseSpectrum(param.getParam("intensity"));
		Point3f p = parsePoint3f(param.getParam("from"));
		Transform l2w = translate(Vector3f(p));
		return std::shared_ptr<PointLight>(new PointLight(l2w, I));
	}
	std::shared_ptr<SpotLight> createSpotLight(const ParamSet & param)
	{
		Spectrum I = parseSpectrum(param.getParam("intensity"));
		Float coneangle = parseFloat(param.getParam("angle"));
		Float conedelta = parseFloat(param.getParam("delta"));
		Point3f from = parsePoint3f(param.getParam("from"));
		Point3f to = parsePoint3f(param.getParam("to"));
		Vector3f dir = normalize(to - from);
		Vector3f z, x;
		coordinateSystem(dir, &z, &x);
		Transform dirToY(Matrix4f(x.x, x.y, x.z, 0.0f, dir.x, dir.y, dir.z, 0.0f, z.x, z.y, z.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		Transform l2w = translate(Vector3f(from)) * inverse(dirToY);
		return std::shared_ptr<SpotLight>(new SpotLight(l2w, I, coneangle, coneangle - conedelta));
	}
	std::shared_ptr<DistantLight> createDistantLight(const ParamSet & param)
	{
		Spectrum L = parseSpectrum(param.getParam("L"));
		Point3f from = parsePoint3f(param.getParam("from"));
		Point3f to = parsePoint3f(param.getParam("to"));
		Vector3f dir = normalize(to - from);
		return std::shared_ptr<DistantLight>(new DistantLight(translate(Vector3f(from)), L, dir));
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
	Spectrum DistantLight::sample_Li(const Intersection & isec, Vector3f * wi, Float * pdf) const
	{
		*wi = -dir;
		*pdf = 1;
		return L;
	}
}
