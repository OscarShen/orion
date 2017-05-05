/************************************************************************ 
 * @description :  light
 * @author		:  Oscar Shen
 * @creat 		:  2017Äê5ÔÂ5ÈÕ17:46:14
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_LIGHT_H_
#define ORION_LIGHT_H_
#include <orion.h>
#include <core/transform.h>
#include <core/spectrum.h>
namespace orion {

	class Light
	{
	protected:
		Transform local2world, world2local;

	public:
		virtual ~Light() {}
		Light(const Transform &light2world)
			: local2world(light2world), world2local(inverse(light2world)) {}
		virtual Spectrum sample_Li(const Intersection &isec, Vector3f *wi, Float *pdf) const = 0;
		virtual void preprocess(const Scene &scene) {}
	};

	class PointLight : public Light
	{
	private:
		const Point3f p;
		const Spectrum I;

	public:
		PointLight(const Transform &light2world, const Spectrum &I)
			: Light(light2world), I(I), p(light2world(Point3f(0))) {}
		virtual Spectrum sample_Li(const Intersection &isec, Vector3f *wi, Float *pdf) const;
	};


	class SpotLight : public Light
	{
	private:
		const Point3f p;
		const Spectrum I;
		const Float cosTotalWidth, cosFalloffStart;

	public:
		SpotLight(const Transform &light2world, const Spectrum &I, Float totalWidth, Float falloffStart);

		virtual Spectrum sample_Li(const Intersection &isec, Vector3f *wi, Float *pdf) const;

		Float falloff(const Vector3f &w) const;
	};

	std::shared_ptr<PointLight> createPointLight(const Transform &light2world, const ParamSet &param);
	std::shared_ptr<SpotLight> createSpotLight(const Transform &light2world, const ParamSet &param);
}

#endif // !ORION_LIGHT_H_
