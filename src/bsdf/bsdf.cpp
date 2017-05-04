#include "bsdf.h"
#include <math/linalg.h>
namespace orion {
	int BSDF::numBxDF(BxDF_TYPE type) const
	{
		int count = 0;
		for (size_t i = 0; i < bxdf.size(); ++i)
			if (bxdf[i]->matchesFlags(type))
				++count;
		return count;
	}
	Spectrum BSDF::f(const Vector3f & wi, const Vector3f & wo, BxDF_TYPE flags) const
	{
		Spectrum r;

		Vector3f swi = world2local(wi); // just rotation, need not to normalize
		Vector3f swo = world2local(wo);

		for (size_t i = 0; i < bxdf.size(); ++i) {
			if(bxdf[i]->matchesFlags(flags))
				r += bxdf[i]->f(swi, swo);
		}
		return r;
	}
	Spectrum BSDF::sample_f(Vector3f & wi, const Vector3f & wo, Float * pdf, BxDF_TYPE type) const
	{
		Vector3f swo = world2local(wo);
		Vector3f swi;

		for (size_t i = 0; i < bxdf.size(); ++i) {
			if (bxdf[i]->matchesFlags(type)) {
				Spectrum t = bxdf[i]->sample_f(swi, swo, pdf);
				wi = local2world(swi);
				return t;
			}
		}
		return Spectrum();
	}
}
