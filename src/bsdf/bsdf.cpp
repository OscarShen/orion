#include "bsdf.h"
#include <math/linalg.h>
#include <sampler/sampler.h>
namespace orion {
	BSDF::BSDF(const Intersection * isec)
	{
		Vector3f sign = cross(isec->dpdu, isec->dpdv);
		nn = dot(sign, isec->n) > 0 ? isec->n : -isec->n;
		sn = normalize(isec->dpdu);
		tn = cross(sn, nn);
	}
	int BSDF::numBxDF(BxDF_TYPE type) const
	{
		int count = 0;
		for (size_t i = 0; i < bxdfs.size(); ++i)
			if (bxdfs[i]->matchesFlags(type))
				++count;
		return count;
	}
	Spectrum BSDF::f(const Vector3f & wi, const Vector3f & wo, BxDF_TYPE flags) const
	{
		Spectrum r;

		Vector3f swi = world2local(wi); // just rotation, need not to normalize
		Vector3f swo = world2local(wo);

		for (size_t i = 0; i < bxdfs.size(); ++i) {
			if(bxdfs[i]->matchesFlags(flags))
				r += bxdfs[i]->f(swi, swo);
		}
		return r;
	}
	Spectrum BSDF::sample_f(Vector3f * wi, const Vector3f & wo, const Point2f &rnd, Float * pdf, BxDF_TYPE type, BxDF_TYPE *sampledType) const
	{
		int matchingComps = numComponents(type);
		if (matchingComps == 0) {
			*pdf = 0;
			if (sampledType) *sampledType = BxDF_TYPE(0);
			return 0;
		}
		int comp = std::min((int)std::floor(rnd[0] * matchingComps), matchingComps - 1);

		const BxDF *bxdf = nullptr;
		int count = comp;
		for (size_t i = 0; i < bxdfs.size(); ++i) {
			if (bxdfs[i]->matchesFlags(type) && count-- == 0) {
				bxdf = bxdfs[i].get();
			}
		}

		Point2f rndRemapped(std::min(rnd[0] * matchingComps - comp, oneMinusEpsilon), rnd[1]);

		// Select a sampling direction randomly.
		Vector3f swi, swo = world2local(wo);
		if (swo.y == 0) return 0;
		*pdf = 0;
		if (sampledType) *sampledType = bxdf->type;
		Spectrum f = bxdf->sample_f(&swi, swo, rndRemapped, pdf);
		if (*pdf == 0) {
			if (sampledType) *sampledType = BxDF_TYPE(0);
			return 0;
		}
		*wi = local2world(swi);

		// Compute overall pdf with all matching bxdfs
		if (!(bxdf->type & BxDF_SPECULAR) && matchingComps > 1)
			for (size_t i = 0; i < bxdfs.size(); ++i)
				if (bxdfs[i].get() != bxdf && bxdfs[i]->matchesFlags(type))
					*pdf += bxdfs[i]->pdf(swi, swo);
		if (matchingComps > 1)
			*pdf /= matchingComps;

		// Compute all contribution for sampled direction
		if (!(bxdf->type & BxDF_SPECULAR) && matchingComps > 1) {
			bool reflect = dot(*wi, nn) * dot(wo, nn) > 0; // TODO : add ng
			f = 0;
			for (size_t i = 0; i < bxdfs.size(); ++i) {
				if (bxdfs[i]->matchesFlags(type) && // match type
					((reflect && (bxdfs[i]->type & BxDF_REFLECTION)) ||
					(!reflect && (bxdfs[i]->type & BxDF_TRANSMISSION)))) {
					f += bxdfs[i]->f(swi, swo);
				}
			}
		}
		return f;
	}
	Float BSDF::pdf(const Vector3f & wi, const Vector3f & wo, BxDF_TYPE type) const
	{
		if (bxdfs.size() == 0) return 0;
		Vector3f swi = world2local(wi), swo = world2local(wo); // only rotation, do not need normalization.
		if (swo.y == 0) return 0;
		Float pdf = 0;
		int matchingComps = 0;
		for (size_t i = 0; i < bxdfs.size(); ++i) {
			if (bxdfs[i]->matchesFlags(type)) {
				++matchingComps;
				pdf += bxdfs[i]->pdf(swi, swo);
			}
		}
		if (matchingComps > 0)
			pdf /= matchingComps;
		return pdf;
	}
}
