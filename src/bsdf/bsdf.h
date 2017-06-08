/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_H_
#define ORION_BSDF_H_
#include <orion.h>
#include "bxdf.h"
#include <math/linalg.h>
#include <core/intersection.h>
namespace orion {

	class BSDF
	{
	private:
		std::vector<std::shared_ptr<BxDF>> bxdfs;
		// Normal3f ng  // geometry normal
		Normal3f nn;	// shading normal 
		Vector3f sn, tn;// tangent vector, bitangent vector

	public:
		BSDF(const Intersection *isec) {
			nn = isec->n;
			sn = normalize(isec->dpdu);
			tn = cross(sn, nn);
		}
		~BSDF() {}
		int numBxDF(BxDF_TYPE type = BxDF_ALL) const;
		void addBxDF(const std::shared_ptr<BxDF> &bxdf) { this->bxdfs.push_back(bxdf); }

		Spectrum f(const Vector3f &wi, const Vector3f &wo, BxDF_TYPE flags = BxDF_ALL) const;
		Spectrum sample_f(Vector3f *wi, const Vector3f &wo, const Point2f &rnd, Float *pdf, BxDF_TYPE type = BxDF_ALL, BxDF_TYPE *sampledType = nullptr) const;
		Vector3f world2local(const Vector3f &v) const {
			return Vector3f(dot(v, sn), dot(v, nn), dot(v, tn)); // coordinate transform
		}
		Vector3f local2world(const Vector3f &v) const {
			return Vector3f(sn.x * v.x + nn.x * v.y + tn.x * v.z,
				sn.y * v.x + nn.y * v.y + tn.y * v.z,
				sn.z * v.x + nn.z * v.y + tn.z * v.z);
		}

		Float pdf(const Vector3f &wi, const Vector3f &wo, BxDF_TYPE type = BxDF_ALL) const;

		int numComponents(BxDF_TYPE type = BxDF_ALL) const;
	};

	inline int BSDF::numComponents(BxDF_TYPE type) const
	{
		int num = 0;
		for (size_t i = 0; i < bxdfs.size(); ++i) {
			if (bxdfs[i]->matchesFlags(type))
				++num;
		}
		return num;
	}
}

#endif // !ORION_BSDF_H_
