/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_INTEGRATOR_SPPM_H_
#define ORION_INTEGRATOR_SPPM_H_
#include <orion.h>
#include "integrator.h"
namespace orion {

	struct SPPMPixel;
	class SPPMIntegrator : public Integrator
	{
	private:
		const Float initSearchRadius;	// means r0
		const int numIter;				// number of iterations
		const int maxDepth;
		const int numPhotonsIter;		// number photons per iteration
		std::shared_ptr<Distribution1D> lightDistrib;

	public:
		SPPMIntegrator(const std::shared_ptr<Camera> &camera, const std::shared_ptr<Sampler> &sampler,
			int maxDepth, int nIterations, int photonPerIteration,
			Float initialSerachRadius);

		virtual void preprocess(const Scene &scene, Sampler &sampler) override;
		virtual void render(const Scene &scene) override;
	};

	std::shared_ptr<SPPMIntegrator> createSPPMIntegrator(const std::shared_ptr<Camera> &camera,
		const std::shared_ptr<Sampler> &sampler, const ParamSet &param);
}

#endif // !ORION_INTEGRATOR_SPPM_H_
