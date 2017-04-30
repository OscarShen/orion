/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_INTEGRATOR_WHITTED_H_
#define ORION_INTEGRATOR_WHITTED_H_
#include <orion.h>
#include <core/integrator.h>
#include <core/spectrum.h>
#include <bsdf/lambert.h>
#include <common/paramset.h>
namespace orion {

	class WhittedIntegrator : public Integrator
	{
	public:
		WhittedIntegrator() {}
		~WhittedIntegrator() {}

		Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene) const override;
	};

	std::shared_ptr<Integrator> createWhittedIntegrator(const ParamSet &param);
}

#endif // !ORION_INTEGRATOR_WHITTED_H_
