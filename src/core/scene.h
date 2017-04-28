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
#include <util/meshmanager.h>
#include <accelerators/kdtree.h>
namespace orion {

	class Scene
	{
	private:
		std::shared_ptr<Accelerator> accel;

	public:
		Scene(std::shared_ptr<Accelerator> accel) : accel(accel) {}
		~Scene() {}

		bool intersect(const Ray &ray, Intersection *isec) const;
	};

}

#endif // !ORION_SCENE_H_
