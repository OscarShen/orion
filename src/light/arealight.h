/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_AREA_LIGHT_H_
#define ORION_AREA_LIGHT_H_
#include <orion.h>
#include "light.h"
namespace orion {

	class AreaLight : public Light
	{
	private:
		const Spectrum Lemit;
		std::shared_ptr<Shape> shape;
		const Float area;

	public:
		AreaLight(const Transform &light2world, const Spectrum &Le,
			int nSamples, const std::shared_ptr<Shape> &shape);
		virtual Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const override;
		virtual Float pdf_Li(const Intersection &isec, const Vector3f &wi) const override;
		Spectrum L(const Intersection &isec, const Vector3f &w) const;
		virtual Spectrum power() const override {
			return Lemit * area * pi;
		}
	};

	std::shared_ptr<AreaLight> createAreaLight(const Transform &light2world, const std::shared_ptr<Shape> &shape, const ParamSet &param);
}

#endif // !ORION_AREA_LIGHT_H_
