/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#include <orion.h>
#include "geometry.h"
#include "triangle.h"

ORION_NAMESPACE_BEGIN

class Primitive
{
public:
	std::shared_ptr<Triangle> triangle;
	std::shared_ptr<Material> material;
	std::shared_ptr<AreaLight> areaLight;

public:
	Primitive(const std::shared_ptr<Triangle> &triangle, const std::shared_ptr<Material> &material,
		const std::shared_ptr<AreaLight> &areaLight = nullptr)
		:triangle(triangle), material(material), areaLight(areaLight) {}
	Primitive(const std::shared_ptr<Triangle> &triangle)
		:triangle(triangle), material(nullptr) {}
	std::shared_ptr<Material> getMaterial() const { return material; }
	void setMaterial(const std::shared_ptr<Material> &material) { this->material = material; }

	const AreaLight * getAreaLight() const { return areaLight.get(); }
};

ORION_NAMESPACE_END