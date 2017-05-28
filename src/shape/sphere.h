/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SHAPE_SPHERE_H_
#define ORION_SHAPE_SPHERE_H_
#include <orion.h>
#include "shape.h"
namespace orion {

	class Sphere : public Shape
	{
	private:
		const Float radius;

	public:
		Sphere(const Transform *local2world, const Transform *world2local,
			Float radius) : Shape(local2world, world2local),
			radius(radius) {}

		virtual bool intersect(const Ray &ray, Intersection *isec) const override;
		virtual Bounds3f localBound() const override;
		virtual Float area() const override;
		virtual Intersection sample(const Point2f &u, Float *pdf) const override;
		virtual Intersection sample(const Intersection &isec, const Point2f &rnd, Float *pdf) const override;

		virtual Float pdf(const Intersection &isec) const { return 1 / area(); }
		virtual Float pdf(const Intersection &isec, const Vector3f &wi) const;
	};

	std::shared_ptr<Sphere> createSphere(const Transform *local2world, const Transform *world2local, const ParamSet &param);

}


#endif // !ORION_SHAPE_SPHERE_H_
