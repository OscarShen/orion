/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_LAMBERT_H_
#define ORION_BSDF_LAMBERT_H_

#include <core/bsdf.h>

ORION_NAMESPACE_BEGIN

class LambertReflection : public BxDF
{
private:
	Spectrum R;

public:
	LambertReflection(const Spectrum &R)
		: BxDF(BxDFType(BxDF_REFLECTION | BxDF_DIFFUSE)), R(R) {}
	Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;
};

class LambertTransmission : public BxDF
{
private:
	Spectrum T;

public:
	LambertTransmission(const Spectrum &T)
		: BxDF(BxDFType(BxDF_TRANSMISSION | BxDF_DIFFUSE)), T(T) {}
	Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rand,
		Float *pdf, BxDFType *sampledType = nullptr) const override;
	Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;
	Float pdf(const Vector3f &swi, const Vector3f &swo) const override;
};

ORION_NAMESPACE_END

#endif // !ORION_BSDF_LAMBERT_H_
