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
		Lambert(const Spectrum &ss) : BxDF(BxDF_TYPE(BxDF_DIFFUSE | BxDF_REFLECTION)), s(ss) { }
		~Lambert() {}

		virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;

		virtual Spectrum sample_f(Vector3f *wi, const Vector3f &wo, Float *pdf) const override;
	};

}

#endif // !ORION_BXDF_LAMBERT_H_
