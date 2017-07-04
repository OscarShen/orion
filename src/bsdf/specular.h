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
#include <math/linalg.h>
#include <physics/optics.h>
namespace orion {

	class SpecularReflection : public BxDF
	{
	private:
		const std::shared_ptr<Fresnel> fresnel;
		const Spectrum R;

	public:
		SpecularReflection(const Spectrum &R, const std::shared_ptr<Fresnel> &fresnel)
			: BxDF(BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR)), R(R),
			fresnel(fresnel) {}
		~SpecularReflection() {}

		virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const override
		{ return Spectrum(0.0f); }

		virtual Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rnd,
			Float *pdf, BxDF_TYPE *sampledType = nullptr) const;

		virtual Float pdf(const Vector3f &swi, const Vector3f &swo) const override { return 0; }
	};

	class SpecularTransmission : public BxDF
	{
	private:
		const Spectrum T;
		const Float etaA, etaB;
		const FresnelDielectric fresnel;
		bool hasAtenuation;

	public:
		SpecularTransmission(const Spectrum &T, Float etaA, Float etaB, bool hasAtenuation = true)
			: BxDF(BxDF_TYPE(BxDF_TRANSMISSION | BxDF_SPECULAR)),
			T(T), etaA(etaA), etaB(etaB), fresnel(etaA, etaB), hasAtenuation(hasAtenuation) {}

		Spectrum f(const Vector3f &swi, const Vector3f &swo) const override { return Spectrum(0.0f); }

		virtual Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rnd,
			Float *pdf, BxDF_TYPE *sampledType = nullptr) const;

		virtual Float pdf(const Vector3f &swi, const Vector3f &swo) const override { return 0; }
	};

	class FresnelSpecular : public BxDF
	{
	private:
		const Spectrum R, T;
		const Float etaA, etaB;
		bool hasAtenuation;

	public:
		FresnelSpecular(const Spectrum &R, const Spectrum &T, Float etaA, Float etaB, bool hasAtenuation = true)
			: BxDF(BxDF_TYPE(BxDF_REFLECTION | BxDF_TRANSMISSION | BxDF_SPECULAR)),
			R(R), T(T), etaA(etaA), etaB(etaB), hasAtenuation(hasAtenuation) {}

		Spectrum f(const Vector3f &swi, const Vector3f &swo) const override { return Spectrum(0.0f); }

		virtual Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rnd,
			Float *pdf, BxDF_TYPE *sampledType = nullptr) const;

		virtual Float pdf(const Vector3f &swi, const Vector3f &swo) const override { return 0; }
	};

}

#endif // !ORION_REFLECTION_H_
