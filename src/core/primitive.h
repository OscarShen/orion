/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_PRIMITIVE_H_
#define ORION_CORE_PRIMITIVE_H_
#include <orion.h>
#include <shape/shape.h>
#include <material/material.h>
namespace orion {

	class Primitive
	{
	private:
		std::shared_ptr<Shape> shape;
		std::shared_ptr<Material> material;
		std::shared_ptr<AreaLight> areaLight; // handle area light pointer if it is emissive.

	public:
		Primitive(const std::shared_ptr<Shape> &shape, const std::shared_ptr<Material> &material)
			:shape(shape), material(material) {}
		Primitive(const std::shared_ptr<Shape> &shape)
			:shape(shape), material(nullptr) {}
		virtual Bounds3f worldBound() const { return shape->worldBound(); }
		virtual bool intersect(const Ray &ray, Intersection *isec) const;
		std::shared_ptr<Material> getMaterial() const { return material; }
		void setMaterial(const std::shared_ptr<Material> &material) { this->material = material; }

		const AreaLight * getAreaLight() const { return areaLight.get(); }
	};

}

#endif // !ORION_CORE_PRIMITIVE_H_
