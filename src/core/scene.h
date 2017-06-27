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
#include "geometry.h"
#include "kernel.h"
ORION_NAMESPACE_BEGIN

class Scene {
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Sampler> sampler;
	std::vector<Light> lights;
	std::unique_ptr<Kernel> kernel;
	Bounds3f bound;

public:
	Scene(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler,
		const std::vector<Primitive> &primitives, const std::vector<Light> &lights)
		: camera(camera), sampler(sampler), lights(lights) {
		// only support embree kernel now
		kernel = std::make_unique<EmbreeKernel>(primitives);
	}
	void build();
	bool intersect(const Ray &ray, Intersection *isec) const;
};

ORION_NAMESPACE_END


#endif // !ORION_SCENE_H_
