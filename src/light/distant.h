/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_LIGHT_DISTANT_H_
#define ORION_LIGHT_DISTANT_H_

#include <core/light.h>
ORION_NAMESPACE_BEGIN

class DistantLight : public Light
{
private:
	const Spectrum L;
	const Vector3f dir;
	Point3f worldCenter;
	Float worldRadius;

public:
	DistantLight(const Transform &light2world, const Spectrum &L, const Vector3f &dir)
		: Light((int)LightType::DeltaDirection, light2world), L(L), dir(normalize(light2world(dir))) {}

	void preprocess(const Scene &scene) override;

	Spectrum power() const override;

	Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const override;
	Float pdf_Li(const Intersection &isec, const Vector3f &wi) const override { return 0; }

	Spectrum sample_Le(const Point2f &rand1, const Point2f &rand2,
		Ray *ray, Normal3f *nLight, Float *pdfPos, Float *pdfDir) const override;
	void pdf_Le(const Ray &ray, const Normal3f &n, Float *pdfPos,
		Float *pdfDir) const override;
};

ORION_NAMESPACE_END

#endif // !ORION_LIGHT_DISTANT_H_
