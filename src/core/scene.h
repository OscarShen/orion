/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SCENE_H_
#define ORION_SCENE_H_
#include <orion.h>
#include <shape/triangle.h>
#include <accelerators/kdtree.h>
namespace orion {

	class Scene
	{
	public:
		std::vector<std::shared_ptr<Light>> lights;

	private:
		std::shared_ptr<Accelerator> accel;
		Bounds3f bound;


	public:
		Scene(std::shared_ptr<Accelerator> accel, const std::vector<std::shared_ptr<Light>> &lights);

		~Scene() {}

		bool intersect(const Ray &ray, Intersection *isec) const;
		Bounds3f worldBound() const { return bound; }
	};

}

#endif // !ORION_SCENE_H_
