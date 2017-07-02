/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_INTEGRATOR_PATH_H_
#define ORION_INTEGRATOR_PATH_H_

#include <core/integrator.h>
ORION_NAMESPACE_BEGIN
struct Distribution1D;
class PathTracing : public Integrator
{
private:
	int maxDepth;
	Distribution1D *lightDistrib;

public:
	PathTracing(const std::shared_ptr<Camera> &camera,
		const std::shared_ptr<Sampler> &sampler, int maxDepth)
		: Integrator(camera, sampler), maxDepth(maxDepth) {}
	~PathTracing() { delete lightDistrib; }
	void preprocess(const Scene &scene, Sampler &sampler) override;
	Spectrum Li(const Ray &ray, const Scene &scene, Sampler &sampler, int depth = 0) const override;
};

std::shared_ptr<PathTracing> createPathTracingIntegrator(const std::shared_ptr<Camera> &camera,
	const std::shared_ptr<Sampler> &sampler, const ParamSet & param);

ORION_NAMESPACE_END

#endif // !ORION_INTEGRATOR_PATH_H_
