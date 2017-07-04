/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_INTEGRATOR_DIRECT_LIGHTING_H_
#define ORION_INTEGRATOR_DIRECT_LIGHTING_H_
#include <orion.h>
#include "integrator.h"
namespace orion {

	class DirectLighting : public Integrator
	{
	private:
		const int maxDepth;
		std::vector<int> nLightSamples;

	public:
		DirectLighting(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, int maxDepth)
			: Integrator(camera, sampler), maxDepth(maxDepth) {}
		virtual void preprocess(const Scene &scene, Sampler &sampler) override;
		virtual Spectrum Li(const Ray &ray, const std::shared_ptr<Scene> &scene,
			const std::shared_ptr<Sampler> &sampler, int depth) const;
	};
	std::shared_ptr<DirectLighting> createDirectLightingIntegrator(std::shared_ptr<Camera> camera,
		std::shared_ptr<Sampler> sampler, const ParamSet &param);
}

#endif // !ORION_INTEGRATOR_DIRECT_LIGHTING_H_
