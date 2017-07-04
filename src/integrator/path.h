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
#include <util/threading.h>
ORION_NAMESPACE_BEGIN
struct Distribution1D;
class PathTracing : public Integrator
{
private:
	int maxDepth;
	int nSamples;
	std::shared_ptr<Distribution1D> lightDistrib;
	Spinlock outputLock;

public:
	PathTracing(const std::shared_ptr<Camera> &camera,
		const std::shared_ptr<Sampler> &sampler, int maxDepth, int nSamples)
		: Integrator(camera, sampler), maxDepth(maxDepth), nSamples(nSamples) {}
	~PathTracing() {}
	void render(const Scene &scene) override;
	void preprocess(const Scene &scene, Sampler &sampler) override;
	Spectrum Li(const Ray &ray, const Scene &scene, Sampler &sampler, int depth = 0) const override;
	
private:
	void accumulate(int i, int j, const Spectrum &s);
};

std::shared_ptr<PathTracing> createPathTracingIntegrator(const std::shared_ptr<Camera> &camera,
	const std::shared_ptr<Sampler> &sampler, const ParamSet & param);

ORION_NAMESPACE_END

#endif // !ORION_INTEGRATOR_PATH_H_
