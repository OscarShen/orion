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

}

#endif // !ORION_INTEGRATOR
