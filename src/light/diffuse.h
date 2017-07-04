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
#include <core/triangle.h>
ORION_NAMESPACE_BEGIN

class DiffuseAreaLight : public AreaLight
{
private:
	Spectrum Le;
	std::shared_ptr<Triangle> triangle;
	Float area;

public:
	DiffuseAreaLight(const Transform &light2world, const Spectrum &Le, int nSamples,
		const std::shared_ptr<Triangle> &tri)
		: AreaLight(light2world, nSamples), Le(Le), triangle(tri), area(triangle->area()) {}
	Spectrum power() const override;

	Spectrum sample_Li(const Intersection &isec, const Point2f &rnd,
		Vector3f *wi, Float *pdf, ShadowTester *sdt, Point3f *samplePoint = nullptr) const override;
	Float pdf_Li(const Intersection &isec, const Vector3f &wi, const Scene &scene) const override;

	Spectrum sample_Le(const Point2f &rand1, const Point2f &rand2,
		Ray *ray, Normal3f *nLight, Float *pdfPos, Float *pdfDir) const override;
	void pdf_Le(const Ray &ray, const Normal3f &n, Float *pdfPos,
		Float *pdfDir) const override;

	Spectrum L(const Intersection &intr, const Vector3f &w) const override;
};

std::shared_ptr<DiffuseAreaLight> createDiffuseAreaLight(const Transform &light2world, const std::shared_ptr<Triangle> &tri, const ParamSet &param);

ORION_NAMESPACE_END

#endif // !ORION_LIGHT_DIFFUSE_H_
