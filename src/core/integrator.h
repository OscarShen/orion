/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_INTEGRATOR_H_
#define ORION_CORE_INTEGRATOR_H_

#include <orion.h>
#include "spectrum.h"
ORION_NAMESPACE_BEGIN

class Integrator
{
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Sampler> sampler;

public:
	virtual ~Integrator() {}
	virtual void render(const Scene &scene) = 0;
	virtual Spectrum Li(const Ray &ray, const Scene &scene, Sampler &sampler, int depth = 0) const {
		CHECK_INFO(false, "no impl!");
		return 0;
	}
};

ORION_NAMESPACE_END

#endif // !ORION_CORE_INTEGRATOR_H_
