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
}
