/************************************************************************ 
 * @description :  camera with thin lens
 * @author		:  Oscar Shen
 * @creat 		:  2017Äê5ÔÂ9ÈÕ16:33:32
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
		ThinLensCamera(const Point3f &orig, const Point3f &lookat, const Vector3f &up, Float vfov,
			const std::shared_ptr<RenderTarget> &film, Float focalDistance, Float lensRadius)
			: PerspectiveCamera(orig, lookat, up, vfov, film), focalDistance(focalDistance), lensRadius(lensRadius)
		{
			setFov(vfov);
			t = lookAt(orig, lookat, up);
		}
		virtual Ray generateRay(const Point2f &offset, const std::shared_ptr<Sampler> &sampler) const override;
	};

	std::shared_ptr<ThinLensCamera> createThinLensCamera(const ParamSet &param);

}

#endif // !ORION_CAMERA_THIN_LENS_H_
