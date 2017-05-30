/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_INTEGRATOR_PATH_TRACING_H_
#define ORION_INTEGRATOR_PATH_TRACING_H_
#include <orion.h>
#include "integrator.h"
#include <sampler/sampling.h>
namespace orion {

	class PathTracing : public Integrator
	{
	private:
		int maxDepth;
		std::shared_ptr<Distribution1D> lightDistrib;

	public:
		PathTracing(const std::shared_ptr<Camera> &camera,
			const std::shared_ptr<Sampler> &sampler, int maxDepth)
			: Integrator(camera, sampler), maxDepth(maxDepth) {}
		virtual void preprocess(const Scene &scene, Sampler &sampler) override;
		virtual Spectrum Li(const Ray &ray, const std::shared_ptr<Scene> &scene,
			const std::shared_ptr<Sampler> &sampler, int depth) const override;
	};

	std::shared_ptr<PathTracing> createPathTracingIntegrator(const std::shared_ptr<Camera> &camera,
		const std::shared_ptr<Sampler> &sampler, const ParamSet & param);
}

#endif // !ORION_INTEGRATOR_PATH_TRACING_H_
