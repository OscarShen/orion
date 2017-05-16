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

	class LambertianReflection : public BxDF
	{
	private:
		Spectrum R;

	public:
		LambertianReflection(const Spectrum &R) : BxDF(BxDF_TYPE(BxDF_DIFFUSE | BxDF_REFLECTION)), R(R) { }

		virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;
	};

	class LambertianTransmission : public BxDF
	{
	private:
		Spectrum T;

	public:
		LambertianTransmission(const Spectrum &T) : BxDF(BxDF_TYPE(BxDF_DIFFUSE | BxDF_TRANSMISSION)), T(T) {}

		virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;
	};
}

#endif // !ORION_BXDF_LAMBERT_H_
