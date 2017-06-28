/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_LIGHT_DIFFUSE_H_
#define ORION_LIGHT_DIFFUSE_H_
#include <core/light.h>
ORION_NAMESPACE_BEGIN

class DiffuseAreaLight : public AreaLight
{
private:
	Spectrum Le;
	std::shared_ptr<Triangle> triangle;
	Float area;

public:
	Spectrum power() const override;

	Spectrum sample_Li(const Intersection &isec, const Point2f &rnd,
		Vector3f *wi, Float *pdf, ShadowTester *sdt, Point3f *samplePoint = nullptr) const override;
	Float pdf_Li(const Intersection &isec, const Vector3f &wi, const Point3f *samplePoint) const override;

	Spectrum sample_Le(const Point2f &rand1, const Point2f &rand2,
		Ray *ray, Normal3f *nLight, Float *pdfPos, Float *pdfDir) const override;
	void pdf_Le(const Ray &ray, const Normal3f &n, Float *pdfPos,
		Float *pdfDir) const override;

	Spectrum L(const Intersection &intr, const Vector3f &w) const override;
};

ORION_NAMESPACE_END

#endif // !ORION_LIGHT_DIFFUSE_H_
