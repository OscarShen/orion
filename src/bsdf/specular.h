/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_SPECULAR_H_
#define ORION_BSDF_SPECULAR_H_

#include <core/bsdf.h>
ORION_NAMESPACE_BEGIN

class SpecularTransmission : public BxDF
{
private:
	Spectrum T;
	Float etaA, etaB; // eta of both sides
	FresnelDielectric fresnel;
	TransportMode mode;

public:
	SpecularTransmission(const Spectrum &T, Float etaA, Float etaB, TransportMode mode)
		:BxDF(BxDFType(BxDF_TRANSMISSION | BxDF_SPECULAR)), T(T),
		etaA(etaA), etaB(etaB), fresnel(etaA, etaB), mode(mode) {}

	Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rand,
		Float *pdf, BxDFType *sampledType) const override;

	// specular
	Float pdf(const Vector3f &swi, const Vector3f &swo) const { return 0; }
	// specular
	Spectrum f(const Vector3f &swi, const Vector3f &swo) const { return 0; }
};

class SpecularReflection : public BxDF
{
private:
	Spectrum R;
	std::shared_ptr<Fresnel> fresnel;

public:
	SpecularReflection(const Spectrum &R, std::shared_ptr<Fresnel> fresnel)
		:BxDF(BxDFType(BxDF_REFLECTION | BxDF_SPECULAR)), R(R), fresnel(fresnel) {}

	Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rand,
		Float *pdf, BxDFType *sampledType) const override;

	// specular
	Float pdf(const Vector3f &swi, const Vector3f &swo) const { return 0; }
	// specular
	Spectrum f(const Vector3f &swi, const Vector3f &swo) const { return 0; }
};

ORION_NAMESPACE_END

#endif // !ORION_BSDF_SPECULAR_H_
