/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BXDF_LAMBERT_H_
#define ORION_BXDF_LAMBERT_H_
#include <orion.h>
#include <core/spectrum.h>
#include "bxdf.h"
namespace orion {

	class Lambert : public BxDF
	{
	private:
		Spectrum s;

	public:
		Lambert(const Spectrum &ss) : s(ss) { type = BxDF_DIFFUSE; }
		~Lambert() {}

		virtual Spectrum f(const Vector3f &wi, const Vector3f &wo) const override;

		virtual Spectrum sample_f(Vector3f &wi, const Vector3f &wo, Float *pdf) const override;
	};

}

#endif // !ORION_BXDF_LAMBERT_H_
