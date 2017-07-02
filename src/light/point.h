/************************************************************************
* @description :
* @author		:  $username$
* @creat 		:  $time$
************************************************************************
* Copyright @ OscarShen 2017. All rights reserved.
************************************************************************/
#pragma once
#ifndef ORION_LIGHT_POINT_H_
#define ORION_LIGHT_POINT_H_
#include <core/light.h>

ORION_NAMESPACE_BEGIN

class PointLight : public Light
{
private:
	Point3f p;
	Spectrum I;

public:
	PointLight(const Transform &light2world, const Spectrum &I)
		: Light((int)LightType::DeltaPosition, light2world),
		p(light2world(Point3f(0))), I(I) {}

	Spectrum power() const override;

	Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi,
		Float *pdf, ShadowTester *sdt, Point3f *samplePoint = nullptr) const override;
	Float pdf_Li(const Intersection &isec, const Vector3f &wi, const Scene &scene) const override { return 0; }

	Spectrum sample_Le(const Point2f &rand1, const Point2f &rand2,
		Ray *ray, Normal3f *nLight, Float *pdfPos, Float *pdfDir) const override;
	void pdf_Le(const Ray &ray, const Normal3f &n, Float *pdfPos,
		Float *pdfDir) const override;
};

ORION_NAMESPACE_END

#endif // !ORION_LIGHT_POINT_H_
