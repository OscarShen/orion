/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CAMERA_PINHOLE_H_
#define ORION_CAMERA_PINHOLE_H_
#include <core/transform.h>
#include <core/camera.h>
ORION_NAMESPACE_BEGIN

class Pinhole : public Camera
{
private:
	Float vfov, tan_half_fov;
	Transform t;
	Float focalDistance, lensRadius;

public:
	Pinhole(const Transform &camera2world, Float vfov, std::shared_ptr<RenderTarget> film,
		Float focalDistance = 0, Float lensRadius = 0)
		:Camera(film), t(camera2world), vfov(vfov), tan_half_fov(std::tan(radians(vfov * 0.5f))),
		focalDistance(focalDistance), lensRadius(lensRadius) {}

	Ray generateRay(const Point2f &offset, const std::shared_ptr<Sampler> &sampler) const override;
};

std::shared_ptr<Pinhole> createPinholeCamera(const Transform &camera2world, const ParamSet &param);

ORION_NAMESPACE_END

#endif // !ORION_CAMERA_PINHOLE_H_
