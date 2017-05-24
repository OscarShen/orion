/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_MICROFACET_H_
#define ORION_BSDF_MICROFACET_H_
#include <orion.h>
#include "bxdf.h"

// Disney "principled" BRDF
// https://disney-animation.s3.amazonaws.com/library/s2012_pbs_disney_brdf_notes_v2.pdf

// GGX https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.html
namespace orion {

	class MicrofacetDistribution
	{
	public:
		virtual ~MicrofacetDistribution() {}
		virtual Float D(const Vector3f &wh) const = 0;
		virtual Float G(const Vector3f &wi, const Vector3f &wo, const Vector3f &wh) const = 0;
		virtual Vector3f sample_wh(const Vector3f &wo, const Point2f &rand) const = 0;
		virtual Float pdf(const Vector3f &wh, const Vector3f &wo) const = 0;
	};

	class GGX : public MicrofacetDistribution
	{
		const Float alpha;

	public:
		GGX(Float alpha)
			: alpha(alpha) {}
		virtual Float D(const Vector3f &wh) const override;
		// smith G
		virtual Float G(const Vector3f &wi, const Vector3f &wo, const Vector3f &wh) const override;
		virtual Vector3f sample_wh(const Vector3f &wo, const Point2f &rand) const override;
		virtual Float pdf(const Vector3f &wh, const Vector3f &wo) const override;
	};

	class MicrofacetReflection : public BxDF
	{
	private:
		const Spectrum R;
		const std::shared_ptr<MicrofacetDistribution> distribution;
		const std::shared_ptr<Fresnel> fresnel;

	public:
		MicrofacetReflection(const Spectrum &R,
			const std::shared_ptr<MicrofacetDistribution> &distribution,
			const std::shared_ptr<Fresnel> &fresnel)
			: BxDF(BxDF_TYPE(BxDF_REFLECTION | BxDF_GLOSSY)), R(R),
			distribution(distribution), fresnel(fresnel) {}

		virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;

		virtual Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rnd,
			Float *pdf, BxDF_TYPE *sampledType = nullptr) const override;

		virtual Float pdf(const Vector3f &swi, const Vector3f &swo) const;
	};
}

#endif // !ORION_BSDF_MICROFACET_H_
