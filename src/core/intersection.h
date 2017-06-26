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
#include <math/linalg.h>
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
		bool front;

	public:
		Intersection() : t(fInfinity) { }
		Intersection(const Point3f &pHit) : pHit(pHit) {} // for shadow
		Intersection(const Point3f &pHit, const Normal3f &normal, const Point2f &uv, Float t)
			:pHit(pHit), n(normal), uv(uv), t(t) {}
		Intersection(const Point3f &pHit, const Normal3f &normal, const Point2f &uv, Float t, const Vector3f &dpdu, const Vector3f &dpdv)
			:pHit(pHit), n(normal), uv(uv), t(t), dpdu(dpdu), dpdv(dpdv) {}
		~Intersection() {}

		Ray spawnRay(const Intersection &isec) const {
			Point3f origin = offsetRayOrigin(pHit, n, isec.pHit - pHit);
			Vector3f dir = isec.pHit - origin;		// This is not normal vector!!
			return Ray(origin, dir, 0, t, 1 - shadowEpsilon);
		}

		Ray spawnRay(const Vector3f &d) const {
			Point3f o = offsetRayOrigin(pHit, n, d);
			return Ray(o, d, 0, t);
		}

		Spectrum Le(const Vector3f &wo) const;
	};

}

#endif // !ORION_INTERSECTION_H_
