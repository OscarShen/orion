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
#include <core/spectrum.h>
#include <integrator/integrator.h>
#include <bsdf/lambert.h>
#include <common/paramset.h>
namespace orion {

	class WhittedIntegrator : public Integrator
	{
	private:
		int maxDepth;

	public:
		WhittedIntegrator(const std::shared_ptr<const Camera> &camera,
						  const std::shared_ptr<Sampler> &sampler, int maxDepth)
		: Integrator(camera, sampler), maxDepth(maxDepth) {}
		~WhittedIntegrator() {}

		Spectrum Li(const Ray &ray, const std::shared_ptr<Scene> &scene, const std::shared_ptr<Sampler> &sampler, int depth) const override;
	};

	std::shared_ptr<Integrator> createWhittedIntegrator(const std::shared_ptr<const Camera> &camera, 
		const std::shared_ptr<Sampler> &sampler, const ParamSet &param);
}

#endif // !ORION_INTEGRATOR_WHITTED_H_
