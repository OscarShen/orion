/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_PHYSICS_OPTICS_H_
#define ORION_PHYSICS_OPTICS_H_
#include <orion.h>
#include <core/spectrum.h>
#include <core/geometry.h>
#include <math/linalg.h>
namespace orion {

	// 绝缘体反射率
	Float frDielectric(Float cosThetaI, Float etaI, Float etaT);

	// 导体反射率
	Spectrum frConductor(Float cosThetaI, const Spectrum &etai, const Spectrum &etat, const Spectrum &k);

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
}

#endif // !ORION_PHYSICS_OPTICS_H_
