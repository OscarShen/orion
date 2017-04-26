/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BXDF_H_
#define ORION_BXDF_H_
#include <orion.h>
#include <core/spectrum.h>
#include <core/geometry.h>
namespace orion {

	class BxDF
	{
	public:
		BxDF() {}
		virtual ~BxDF() {}

		// evaluate bxdf
		// param wi : in vector, normalized
		// param wo : out vector, normalized
		// return : portion in wo from wi
		virtual Spectrum f(const Vector3f &wi, const Vector3f &wo) const = 0;

		virtual Spectrum sample_f(Vector3f &wi, const Vector3f &wo, Float *pdf) const = 0;
	};

}

#endif // !ORION_BXDF_H_
