/************************************************************************ 
 * @description :  camera with thin lens
 * @author		:  Oscar Shen
 * @creat 		:  2017��5��9��16:33:32
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CAMERA_THIN_LENS_H_
#define ORION_CAMERA_THIN_LENS_H_

#include <orion.h>
#include "camera.h"
#include "perspective.h"
namespace orion {

	class ThinLensCamera : public PerspectiveCamera
	{
	private:
		Float focalDistance = 0, lensRadius = 0;

	public:
		ThinLensCamera() { }
		ThinLensCamera(const Transform &camera2world, Float vfov,
			const std::shared_ptr<RenderTarget> &film, Float focalDistance, Float lensRadius)
			: PerspectiveCamera(camera2world, vfov, film), focalDistance(focalDistance),
			lensRadius(lensRadius) {}
		virtual Ray generateRay(const Point2f &offset, const std::shared_ptr<Sampler> &sampler) const override;
	};

	std::shared_ptr<ThinLensCamera> createThinLensCamera(const Transform &camera2world, const ParamSet &param);
}

#endif // !ORION_CAMERA_THIN_LENS_H_
