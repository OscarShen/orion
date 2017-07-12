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

#include <core/integrator.h>
ORION_NAMESPACE_BEGIN

class SPPM : public Integrator
{
private:
	Float initialSearchRadius;
	Float nIterations;       // times of iterations
	int maxDepth;
	int photonPerIteration;

public:
	SPPM(const std::shared_ptr<Camera> &camera,
		const std::shared_ptr<Sampler> &sampler,
		int maxDepth, int nIterations, int photonsPerIter, Float radius)
		:Integrator(camera, sampler), maxDepth(maxDepth), nIterations(nIterations),
		initialSearchRadius(radius), photonPerIteration(photonsPerIter) {}
	void render(const Scene &scene) override;
};

std::shared_ptr<SPPM> createSPPMIntegrator(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Sampler>& sampler, const ParamSet & param);

ORION_NAMESPACE_END
#endif // !ORION_INTEGRATOR_SPPM_H_
