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
		Point2f uv;

	public:
		Intersection() { }
		Intersection(const Point3f &pHit, const Normal3f &normal, const Point2f &uv)
			:pHit(pHit), n(normal), uv(uv) {}
		~Intersection() {}
	};

}

#endif // !ORION_INTERSECTION_H_
