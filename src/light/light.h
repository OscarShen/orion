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
#include <core/intersection.h>
namespace orion {

	class ShadowTester
	{
	private:
		Intersection p0, p1;

	public:
		ShadowTester() {}
		ShadowTester(const Intersection &isec0, const Intersection &isec1)
			: p0(isec0), p1(isec1) {}
		bool unoccluded(const std::shared_ptr<Scene> & scene) const;
		bool unoccluded(const Scene & scene) const;
	};

	enum class LightType : int
	{
		DeltaPosition = 1,
		DeltaDirection = 2,
		Area = 4,
		Infinite = 8
	};

	inline bool isDeltaLight(int type) {
		return type & (int)LightType::DeltaPosition ||
			type & (int)LightType::DeltaDirection;
	}

	class Light
	{
	public:
		const int nSamples; // samples needed for one light
		const int type;
	protected:
		Transform local2world, world2local;

	public:
		virtual ~Light() {}
		Light(int type, const Transform &light2world, int nSamples = 1)
			: type(type), local2world(light2world), world2local(inverse(light2world)), nSamples(std::max(1, nSamples)) {}
		virtual Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const = 0;
		virtual Float pdf_Li(const Intersection &isec, const Vector3f &wi) const = 0;
		virtual void preprocess(const Scene &scene) {}
		virtual Spectrum power() const = 0;
	};

	class PointLight : public Light
	{
	private:
		const Point3f p;
		const Spectrum I;

	public:
		PointLight(const Transform &light2world, const Spectrum &I)
			: Light((int)LightType::DeltaPosition, light2world), I(I), p(light2world(Point3f(0))) {}

		virtual Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const override;
		virtual Float pdf_Li(const Intersection &, const Vector3f &) const override { return 0; }
		virtual Spectrum power() const override { return 4 * pi * I; }
	};


	class SpotLight : public Light
	{
	private:
		const Point3f p;
		const Spectrum I;
		const Float cosTotalWidth, cosFalloffStart;

	public:
		SpotLight(const Transform &light2world, const Spectrum &I, Float totalWidth, Float falloffStart);

		virtual Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const override;
		virtual Float pdf_Li(const Intersection &isec, const Vector3f &wi) const override { return 0; }
		virtual Spectrum power() const override {
			return I * 2 * pi * (1 - 0.5f * (cosFalloffStart + cosTotalWidth));
		}

		Float falloff(const Vector3f &w) const;
	};

	class DistantLight : public Light
	{
	private:
		const Spectrum L;
		const Vector3f dir;
		Point3f worldCenter;
		Float worldRadius;

	public:
		DistantLight(const Transform &light2world, const Spectrum &L, const Vector3f &dir)
			: Light((int)LightType::DeltaDirection, light2world), L(L), dir(dir) {}

		virtual void preprocess(const Scene &scene);
		virtual Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const override;
		virtual Float pdf_Li(const Intersection &isec, const Vector3f &wi) const override { return 0; }
		virtual Spectrum power() const override;
	};

	std::shared_ptr<PointLight> createPointLight(const ParamSet &param);
	std::shared_ptr<SpotLight> createSpotLight(const ParamSet &param);
	std::shared_ptr<DistantLight> createDistantLight(const ParamSet &param);
}

#endif // !ORION_LIGHT_H_
