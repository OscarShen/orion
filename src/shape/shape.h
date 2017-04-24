/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SHAPE_H_
#define ORION_SHAPE_H_
#include <orion.h>
#include <core/geometry.h>
#include <core/intersection.h>
namespace orion {

	class Shape
	{
	public:
		Shape() {}
		virtual ~Shape() {}
		virtual bool intersect(const Ray &ray, Intersection *isec) const = 0;
		virtual Bounds3f worldBound() const = 0;
	};

}

#endif // !ORION_SHAPE_H_
