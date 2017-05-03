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
namespace orion {

	class BSDF
	{
	private:
		std::vector<std::shared_ptr<BxDF>> bxdf;
		// Normal3f ng  // geometry normal
		Normal3f nn;	// shading normal 
		Vector3f sn, tn;// tangent vector, bitangent vector

	public:
		BSDF() {}
		~BSDF() {}
		int numBxDF() const { return static_cast<int>(bxdf.size()); }
		void addBxDF(const std::shared_ptr<BxDF> &bxdf) { this->bxdf.push_back(bxdf); }
		void setIntersection(const Intersection *isec);

		Spectrum f(const Vector3f &wi, const Vector3f &wo, BxDF_TYPE flags = BxDF_ALL) const;
		Vector3f world2local(const Vector3f &v) const {
			return Vector3f(dot(v, sn), dot(v, nn), dot(v, tn)); // coordinate transform
		}
	};

}

#endif // !ORION_BSDF_H_
