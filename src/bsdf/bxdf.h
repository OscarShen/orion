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
		BxDF_TRANSMISSION = 16,
		BxDF_ALL_TYPES = BxDF_DIFFUSE | BxDF_GLOSSY | BxDF_SPECULAR,
		BxDF_ALL_REFLECTION = BxDF_ALL_TYPES | BxDF_REFLECTION,
		BxDF_ALL_TRANSMISSION = BxDF_ALL_TYPES | BxDF_TRANSMISSION,
		BxDF_ALL = BxDF_ALL_REFLECTION | BxDF_ALL_TRANSMISSION
	};

	class BxDF
	{
	public:
		BxDF_TYPE type;


		BxDF(BxDF_TYPE type) : type(type) {}
		virtual ~BxDF() {}

		bool matchesFlags(BxDF_TYPE t) const {
			return (type & t) == type;
		}

		// evaluate bxdf
		// param wi : in vector, normalized
		// param wo : out vector, normalized
		// return : portion in wo from wi
		virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const = 0;

		virtual Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rnd,
			Float *pdf, BxDF_TYPE *sampledType = nullptr) const;

		virtual Float pdf(const Vector3f &swi, const Vector3f &swo) const;
	};

	inline bool refract(const Vector3f &wi, const Normal3f &n, Float eta, Vector3f *wt) {
		// default normal vectors
		// Snell's law : sinThetaI * etaI = sinThetaT * etaT
		// note : y of wi is positive!
		Float cosThetaI = dot(wi, n);
		Float sin2ThetaI = std::max(Float(0), Float(1 - cosThetaI * cosThetaI));
		Float sin2ThetaT = eta * eta * sin2ThetaI;

		// Is this total internal reflection
		if (sin2ThetaT >= 1) return false;
		Float cosThetaT = std::sqrt(1 - sin2ThetaT);
		*wt = eta * -wi + (eta * cosThetaI - cosThetaT) * Vector3f(n);
		return true;
	}

	inline Vector3f reflect(const Vector3f &wo, const Vector3f &n) {
		return -wo + 2 * dot(wo, n) * n;
	}
}

#endif // !ORION_BXDF_H_
