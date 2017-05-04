/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_INTEGRATOR
#define ORION_INTEGRATOR
#include <orion.h>
#include "scene.h"
namespace orion {

	class Integrator
	{
	public:
		virtual ~Integrator() {}
		virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene) const = 0;
		Spectrum specularReflect(const Ray &ray, const Intersection *isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const;
		Spectrum specularTransmit(const Ray &ray, const Intersection *isec, const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const;
	};

}

#endif // !ORION_INTEGRATOR
