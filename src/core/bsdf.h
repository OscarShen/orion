/************************************************************************ 
 * @description :  bsdf base class
 * @author		:  Oscar Shen
 * @creat 		:  2017-6-29 14:27:36
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_BSDF_H_
#define ORION_CORE_BSDF_H_

#include <orion.h>
#include "intersection.h"
ORION_NAMESPACE_BEGIN

enum BxDFType {
	BxDF_REFLECTION = 1 << 0,
	BxDF_TRANSMISSION = 1 << 1,
	BxDF_DIFFUSE = 1 << 2,
	BxDF_GLOSSY = 1 << 3,
	BxDF_SPECULAR = 1 << 4,
	BxDF_ALL = BxDF_DIFFUSE | BxDF_GLOSSY | BxDF_SPECULAR | BxDF_REFLECTION |
	BxDF_TRANSMISSION,
};

class BxDF
{
public:
	BxDFType type;

public:
	virtual ~BxDF() {}
	BxDF(BxDFType type) : type(type) {}
	bool matchType(BxDFType t) const { return (type & t) == type; }

	// use cosine hemisphere reflection sample by default
	virtual Spectrum sample_f(Vector3f *swi, const Vector3f &swo, const Point2f &rand,
		Float *pdf, BxDFType *sampledType = nullptr) const;
	// use cosine hemisphere reflection pdf by default
	virtual Float pdf(const Vector3f &swi, const Vector3f &swo) const;
	virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const = 0;
};

class BSDF
{
private:
	Normal3f ng, ns;
	Vector3f u, v; // u : shading x axis, v : y, ns : z
	std::vector<std::shared_ptr<BxDF>> bxdfs;

public:
	BSDF(const Intersection &isec) : ng(isec.ng), ns(isec.ns), u(isec.u), v(isec.v) {}
	int numBxDF(BxDFType type = BxDF_ALL) const;
	void addBxDF(const std::shared_ptr<BxDF> &bxdf) { this->bxdfs.push_back(bxdf); }

	Vector3f world2local(const Vector3f &w) const;
	Vector3f local2world(const Vector3f &w) const;

	Spectrum sample_f(Vector3f *wi, const Vector3f &wo, const Point2f &rand,
		Float *pdf, BxDFType type = BxDF_ALL, BxDFType *sampledType = nullptr) const;
	Spectrum pdf(const Vector3f &wi, const Vector3f &wo, BxDFType type = BxDF_ALL) const;
};

inline Vector3f BSDF::world2local(const Vector3f & w) const
{
	return Vector3f(dot(w, u), dot(w, v), dot(w, ns));
}

inline Vector3f BSDF::local2world(const Vector3f & w) const
{
	return Vector3f(u.x * w.x + v.x * w.y + ns.x * w.z,
					u.y * w.x + v.y * w.y + ns.y * w.z,
					u.z * w.x + v.z * w.y + ns.z * w.z);
}

ORION_NAMESPACE_END
#endif // !ORION_CORE_BSDF_H_
