/************************************************************************ 
 * @description :  perspective camera
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-22 11:55:44
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_PERSPECTIVE_CAMEREA_H_
#define ORION_PERSPECTIVE_CAMEREA_H_
#include <orion.h>
#include <common/paramset.h>
#include <core/geometry.h>
#include <core/transform.h>
#include "camera.h"
namespace orion {

	class PerspectiveCamera : public Camera
	{
	protected:
		Float vfov, tan_half_fov;		// Vertical fov of camera, and a good value for calculation
		Transform t;
		Float focalDistance = 0, lensRadius = 0;

	public:
		PerspectiveCamera() {}
		PerspectiveCamera(const Transform &camera2world, Float vfov,
			const std::shared_ptr<RenderTarget> &film, Float focalDistance, Float lensRadius)
			: Camera(film), t(camera2world), vfov(vfov), tan_half_fov(std::tan(radians(vfov * 0.5f))), focalDistance(focalDistance),
			lensRadius(lensRadius) {}

		~PerspectiveCamera() {}
		
		virtual Ray generateRay(const Point2f &offset, const std::shared_ptr<Sampler> &sampler) const override;

		virtual Spectrum We(const Ray &ray, Point2f *pRaster2 = nullptr) const override;
		virtual void Pdf_We(const Ray &ray, Float *pdfPos, Float *pdfDir) const override;
		virtual Spectrum Sample_Wi(const Intersection &ref, const Point2f &u,
			Vector3f *wi, Float *pdf, Point2f *pRaster, ShadowTester *vis) const override;
	};

	std::shared_ptr<Camera> createPerspectiveCamera(const Transform &camera2world, const ParamSet &param);
}

#endif // !ORION_PERSPECTIVE_CAMEREA_H_
