#include "bsdf.h"
#include <core/intersection.h>
#include <math/linalg.h>
namespace orion {
	void BSDF::setIntersection(const Intersection * isec)
	{
		nn = isec->n;
		sn = normalize(isec->dpdu);
		tn = cross(sn, nn);
	}
	Spectrum BSDF::f(const Vector3f & wi, const Vector3f & wo) const
	{
		Spectrum r;

		Vector3f swi = world2local(wi);
		swi = normalize(swi);
		Vector3f swo = world2local(wo);
		swo = normalize(swo);

		for (size_t i = 0; i < bxdf.size(); ++i) {
			r += bxdf[i]->f(swi, swo);
		}
		return r;
	}
}
