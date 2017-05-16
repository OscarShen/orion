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
#include <core/scene.h>
#include <camera/camera.h>
namespace orion {

	class Integrator
	{
	protected:
		std::shared_ptr<const Camera> camera;

	private:
		std::shared_ptr<Sampler> sampler;

	public:
		Integrator(const std::shared_ptr<const Camera> &camera,
			const std::shared_ptr<Sampler> &sampler)
			: camera(camera), sampler(sampler) {}
		virtual ~Integrator() {}
		virtual Spectrum Li(const Ray &ray, const std::shared_ptr<Scene> &scene,
			const std::shared_ptr<Sampler> &sampler, int depth) const = 0;
		Spectrum specularReflect(const Ray &ray, const Intersection *isec, const std::shared_ptr<Sampler> &sampler,
			const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const;
		Spectrum specularTransmit(const Ray &ray, const Intersection *isec, const std::shared_ptr<Sampler> &sampler,
			const std::shared_ptr<BSDF> &bsdf, const std::shared_ptr<Scene> &scene, int depth) const;
	};

	Spectrum uniformSampleAllLights(const Ray &ray, const Intersection &isec, const Scene &scene, Sampler &sampler, int nSamples);

	Spectrum estimateDirect(const Ray &ray, const Intersection &isec, const Point2f &BSDFSample, const Light &light,
		const Point2f &lightSample, const Scene &scene, Sampler &sampler, BxDF_TYPE type = BxDF_ALL);
}

#endif // !ORION_INTEGRATOR
