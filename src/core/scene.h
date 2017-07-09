/************************************************************************ 
 * @description :  Scene v2.0
 * @author		:  Oscar Shen
 * @creat 		:  2017-6-27 16:38:09
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SCENE_H_
#define ORION_SCENE_H_
#include <orion.h>
#include "kernel.h"
#include "geometry.h"
ORION_NAMESPACE_BEGIN

class Scene {
public:
	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<Light>> envmaps;

private:
	std::unique_ptr<Kernel> kernel;

public:
	Scene(const std::vector<std::shared_ptr<Primitive>> &primitives, const std::vector<std::shared_ptr<Light>> &lights,
		const std::vector<std::shared_ptr<Light>> &envmaps)
		: lights(lights), envmaps(envmaps) {
		// only support embree kernel now
		kernel = std::make_unique<EmbreeKernel>(primitives);
	}
	bool intersect(const Ray &ray, Intersection *isec) const;
	Bounds3f worldbound() const { return kernel->worldBound(); }
};

ORION_NAMESPACE_END


#endif // !ORION_SCENE_H_
