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
namespace orion {

	class Fresnel
	{
	public:
		virtual ~Fresnel() {}
		virtual Spectrum evaluate(Float cosI) const = 0;
	};

	class FresnelConductor : public Fresnel
	{
	private:
		Spectrum etaI, etaT, k;

	public:
		FresnelConductor(const Spectrum &etaI, const Spectrum &etaT, const Spectrum &k)
			: etaI(etaI), etaT(etaT), k(k) {}
		Spectrum evaluate(Float cosThetaI) const override;
	};

	class FresnelDielectric : public Fresnel
	{
	private:
		Float etaI, etaT;

	public:
		FresnelDielectric(Float etaI, Float etaT) : etaI(etaI), etaT(etaT) {}
		Spectrum evaluate(Float cosThetaI) const override;
	};

	class FresnelNoOp : public Fresnel
	{
	public:
		Spectrum evaluate(Float) const { return Spectrum(1.0f); }
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

	class SpecularReflection : public BxDF
	{
	private:
		const std::shared_ptr<Fresnel> fresnel;

	public:
		SpecularReflection(const std::shared_ptr<Fresnel> &fresnel)
			: BxDF(BxDF_TYPE(BxDF_REFLECTION | BxDF_SPECULAR)),
			fresnel(fresnel) {}
		~SpecularReflection() {}

		virtual Spectrum f(const Vector3f &wi, const Vector3f &wo) const override
		{ return Spectrum(0.0f); }

		virtual Spectrum sample_f(Vector3f &wi, const Vector3f &wo, Float *pdf) const override;
	};

	class SpecularTransmission : public BxDF
	{
	private:
		const Spectrum T;
		const Float etaA, etaB;
		const FresnelDielectric fresnel;

	public:
		SpecularTransmission(const Spectrum &T, Float etaA, Float etaB)
			: BxDF(BxDF_TYPE(BxDF_TRANSMISSION | BxDF_SPECULAR)),
			T(T), etaA(etaA), etaB(etaB), fresnel(etaA, etaB) {}

		Spectrum f(const Vector3f &wi, const Vector3f &wo) const override { return Spectrum(0.0f); }

		virtual Spectrum sample_f(Vector3f &wi, const Vector3f &wo, Float *pdf) const override;
	};

}

#endif // !ORION_REFLECTION_H_
