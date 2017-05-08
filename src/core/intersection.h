/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_INTERSECTION_H_
#define ORION_INTERSECTION_H_
#include <orion.h>
#include "geometry.h"
namespace orion {

	class Intersection
	{
	public:
		Point3f pHit;
		Normal3f n;
		Vector3f dpdu, dpdv;
		Point2f uv;
		Float t;
		const Primitive *primitive = nullptr;

	public:
		Intersection() : t(fInfinity) { }
		Intersection(const Point3f &pHit) : pHit(pHit) {} // for shadow
		Intersection(const Point3f &pHit, const Normal3f &normal, const Point2f &uv, Float t)
			:pHit(pHit), n(normal), uv(uv), t(t) {}
		Intersection(const Point3f &pHit, const Normal3f &normal, const Point2f &uv, Float t, const Vector3f &dpdu, const Vector3f &dpdv)
			:pHit(pHit), n(normal), uv(uv), t(t), dpdu(dpdu), dpdv(dpdv) {}
		~Intersection() {}

		Ray spawnRay(const Intersection &isec) const {
			Vector3f dir = isec.pHit - pHit;		// This is not normal vector!!
			Point3f origin = pHit + dir * epsilon;
			return Ray(origin, dir, 0, t, 1 - shadowEpsilon);
		}
	};

}

#endif // !ORION_INTERSECTION_H_
