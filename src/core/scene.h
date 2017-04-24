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
namespace orion {

	class Scene
	{
	private:
		std::vector<std::shared_ptr<Shape>> shapes;

	public:
		Scene() {}
		~Scene() {}

		// Load scene from script file
		bool loadScene(const std::string &name);
		bool intersect(const Ray &ray, Intersection *isec) const;

	private:
		bool _bruteForce(const Ray &ray, Intersection *isec) const;
	};

}

#endif // !ORION_SCENE_H_
