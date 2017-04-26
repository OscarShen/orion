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
		virtual Spectrum Li(const Ray &ray, const Scene &scene) const = 0;
	};

}

#endif // !ORION_INTEGRATOR
