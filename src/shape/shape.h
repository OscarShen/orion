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
#include <core/transform.h>
namespace orion {

	class Shape
	{
	public:
		const Transform *local2world, *world2local;

	public:
		Shape(const Transform *local2world, const Transform *world2local)
			: local2world(local2world), world2local(world2local) {}
		virtual ~Shape() {}
		virtual bool intersect(const Ray &ray, Intersection *isec) const = 0;
		virtual Bounds3f localBound() const = 0;
		virtual Bounds3f worldBound() const;
		virtual Float area() const = 0;

		virtual Intersection sample(const Point2f &rnd, Float *pdf) const = 0;
		virtual Intersection sample(const Intersection &isec, const Point2f &rnd, Float *pdf) const;

		virtual Float pdf(const Intersection &isec) const { return 1 / area(); }
		virtual Float pdf(const Intersection &isec, const Vector3f &wi) const;
	};
}

#endif // !ORION_SHAPE_H_
