/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_BSSRDF_H_
#define ORION_CORE_BSSRDF_H_

#include <orion.h>
#include <core/geometry.h>
#include <core/light.h>
#include <core/intersection.h>
ORION_NAMESPACE_BEGIN
extern Float frDielectric(Float cosThetaI, Float etaI, Float etaT);
Float FresnelMoment1(Float eta);
Float FresnelMoment2(Float eta);

struct BSSRDFTable
{
	const int nRhoSamples, nRadiusSamples;
	std::unique_ptr<Float[]> rhoSamples, radiusSamples;
	std::unique_ptr<Float[]> profile;
	std::unique_ptr<Float[]> rhoEff;
	std::unique_ptr<Float[]> profileCDF;

	BSSRDFTable(int nRhoSamples, int nRadiusSamples);
	inline Float evalProfile(int rhoIndex, int radiusIndex) const {
		return profile[rhoIndex * nRadiusSamples + radiusIndex];
	}
};


class BSSRDF
{
private:
	const Intersection &po;
	Float eta;
	const Normal3f ns;
	const Vector3f u, v;
	const TransportMode mode;

public:
	BSSRDF(const Intersection &po, Float eta, TransportMode mode)
		:po(po), eta(eta), ns(po.ns), u(po.u), v(po.v), mode(mode) {}
	virtual ~BSSRDF() {}

	Spectrum S(const Intersection &pi, const Vector3f &wi) {
		// two ft and hundreds of scattering
		Float fr = frDielectric(cosTheta(po.wo), 1, eta);
		return (1 - fr) * Rd((pi.p - po.p).lengthSquared()) * ft2;
	}
	Spectrum ft2(const Vector3f &w) const {
		Float c = 1 - 2 * FresnelMoment1(1 / eta);
		return (1 - frDielectric(cosTheta(w), 1, eta)) / (c * pi);
	}

	virtual Spectrum Rd(Float d) const = 0;
};

class TabulatedBSSRDF : public BSSRDF
{
private:
	std::shared_ptr<BSSRDFTable> table;
	Spectrum sigma_t, rho;

public:
	TabulatedBSSRDF(const Intersection &po, Float eta, TransportMode mode, const Spectrum &sigma_a,
		const Spectrum &sigma_s, const std::shared_ptr<BSSRDFTable> &table)
		: BSSRDF(po, eta, mode), table(table) {
		sigma_t = sigma_a + sigma_s;
		for (int c = 0; c < 3; ++c)
			rho[c] = sigma_t[c] != 0 ? (sigma_s[c] / sigma_t[c]) : 0;
	}
	Spectrum Rd(Float d) const override;

};
ORION_NAMESPACE_END

#endif // !ORION_CORE_BSSRDF_H_
