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
namespace orion {

	class BSDF
	{
	private:
		std::vector<std::shared_ptr<BxDF>> bxdf;

	public:
		BSDF() {}
		~BSDF() {}
		int numBxDF() const { return static_cast<int>(bxdf.size()); }
		void addBxDF(const std::shared_ptr<BxDF> &bxdf) { this->bxdf.push_back(bxdf); }

		Spectrum f(const Vector3f &wi, const Vector3f &wo) const;
	};

}

#endif // !ORION_BSDF_H_
