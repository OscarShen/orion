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

	enum BxDF_TYPE
	{
		BxDF_DIFFUSE = 1,
		BxDF_GLOSSY = 2,
		BxDF_SPECULAR = 4,
		BxDF_REFLECTION = 8,
		BxDF_REFRACTION = 16,
		BxDF_ALL_TYPES = BxDF_DIFFUSE | BxDF_GLOSSY | BxDF_SPECULAR,
		BxDF_ALL_REFLECTION = BxDF_ALL_TYPES | BxDF_REFLECTION,
		BxDF_ALL_REFRACTION = BxDF_ALL_TYPES | BxDF_REFRACTION,
		BxDF_ALL = BxDF_ALL_REFLECTION | BxDF_ALL_REFRACTION
	};

	class BxDF
	{
	protected:
		int type;

	public:
		BxDF() {}
		virtual ~BxDF() {}

		bool matchesFlags(BxDF_TYPE t) const { return (type & t) == type; }

		// evaluate bxdf
		// param wi : in vector, normalized
		// param wo : out vector, normalized
		// return : portion in wo from wi
		virtual Spectrum f(const Vector3f &wi, const Vector3f &wo) const = 0;

		virtual Spectrum sample_f(Vector3f &wi, const Vector3f &wo, Float *pdf) const = 0;
	};

}

#endif // !ORION_BXDF_H_
