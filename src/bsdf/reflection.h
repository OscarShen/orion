/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_REFLECTION_H_
#define ORION_REFLECTION_H_
#include <orion.h>
#include "bxdf.h"
namespace orion {

	class Reflection : public BxDF
	{
	public:
		Reflection() {}
		~Reflection() {}

		virtual Spectrum f(const Vector3f &wi, const Vector3f &wo) const override;

		virtual Spectrum sample_f(Vector3f &wi, const Vector3f &wo, Float *pdf) const override;
	};

}

#endif // !ORION_REFLECTION_H_
