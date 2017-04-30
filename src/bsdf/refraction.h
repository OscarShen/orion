/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_REFRACTION_H_
#define ORION_BSDF_REFRACTION_H_
#include <orion.h>
#include "bxdf.h"
namespace orion {

	class Refraction : public BxDF
	{
	public:
		Refraction() { type = BxDF_REFLECTION; }
		~Refraction() {}

		virtual Spectrum f(const Vector3f &wi, const Vector3f &wo) const override;

		virtual Spectrum sample_f(Vector3f &wi, const Vector3f &wo, Float *pdf) const override;
	};

}

#endif // !ORION_BSDF_REFRACTION_H_
