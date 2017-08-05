/************************************************************************
 * @description :  intersection info of hit point, v2.0
 * @author		:  Oscar Shen
 * @creat 		:  2017-6-26 22:03:15
************************************************************************
 * Copyright @ OscarShen 2017. All rights reserved.
************************************************************************/
#pragma once
#ifndef ORION_INTERSECTION_H_
#define ORION_INTERSECTION_H_
#include <orion.h>
#include "geometry.h"
#include "spectrum.h"
#include "primitive.h"
#include "material.h"
ORION_NAMESPACE_BEGIN

class Intersection
{
public:
	Point3f p;
	Normal3f ng, ns; // geometry normal, shading normal
	//Vector3f dpdu, dpdv;
	Vector3f u, v; // shading coordinate x axis and y axis
	Point2f uv; // texture uv
	Float t; // value of how far ray hit _p_
	const Primitive *primitive; // hitted primitive
	bool front; // ray entering with direction of _ng_ is true
	std::shared_ptr<BSDF> bsdf;
	Vector3f wo;

public:
	Intersection() { primitive = nullptr; }
	Intersection(const Point3f &p) : p(p) {} // for shadow ray
	Intersection(const Point3f &pHit, const Normal3f &ng, const Normal3f &ns, const Point2f &uv, Float t, bool front)
		:p(pHit), ng(ng), ns(ns), uv(uv), t(t), front(front) {}
	~Intersection() {}

	Ray spawnRay(const Intersection &isec) const {
		Point3f origin = offsetRayOrigin(p, ng, isec.p - p);
		Vector3f dir = isec.p - origin;		// This is not unit vector!!
		return Ray(origin, dir, 0, 1 - shadowEpsilon);
	}

	Ray spawnRay(const Vector3f &d) const {
		Point3f o = offsetRayOrigin(p, ng, d);
		return Ray(o, d, 0, fInfinity);
	}

	Spectrum Le(const Vector3f &wo) const;

	void calculateBSDF(TransportMode mode = TransportMode::Path) {
		bsdf = primitive->material->getBSDF(*this, mode);
	}
};

ORION_NAMESPACE_END

#endif // !ORION_INTERSECTION_H_
