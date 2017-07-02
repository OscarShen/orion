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
#include "bsdf.h"
ORION_NAMESPACE_BEGIN
struct Distribution1D;
class Integrator
{
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Sampler> sampler;

public:
	Integrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler)
		: camera(camera), sampler(sampler) {}
	virtual ~Integrator() {}
	virtual void preprocess(const Scene &scene, Sampler &sampler) {}
	virtual void render(const Scene &scene) {}
	virtual Spectrum Li(const Ray &ray, const Scene &scene, Sampler &sampler, int depth = 0) const {
		CHECK_INFO(false, "no impl!");
		return 0;
	}
};

Spectrum uniformSampleOneLight(const Ray &ray, const Intersection &isec, const Scene &scene,
	Sampler &sampler, const Distribution1D *lightDistrib = nullptr);

Spectrum estimateDirect(const Ray &ray, const Intersection &isec, const Point2f &BSDFSample, const Light &light,
	const Point2f &lightSample, const Scene &scene, Sampler &sampler, BxDFType type = BxDF_ALL);

ORION_NAMESPACE_END

#endif // !ORION_CORE_INTEGRATOR_H_
