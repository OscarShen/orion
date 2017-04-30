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
	private:
		Point3f lookat; // Point which camera look at.
		Vector3f up;	// Up vector of camera
		Float vfov, tan_half_fov;		// Vertical fov of camera, and a good value for calculation

	public:
		PerspectiveCamera() {}
		PerspectiveCamera(const Point3f &orig, const Point3f &lookat, const Vector3f &up, Float vfov, const std::shared_ptr<RenderTarget> &film)
			: Camera(orig, film), lookat(lookat), up(up)
		{
			setFov(vfov);
		}
		~PerspectiveCamera() {}
		
		virtual Ray generateRay(int x, int y) const override;
		void setLookat(const Point3f &lookat) { this->lookat = lookat; }
		void setUp(const Vector3f &up) { this->up = up; }
		void setFov(Float vfov) {
			this->vfov = vfov;
			tan_half_fov = std::tan(radians(vfov * 0.5f));
		}
		Float getFov() const { return vfov; }
	};

	std::shared_ptr<Camera> createPerspectiveCamera(const ParamSet &param);
}

#endif // !ORION_PERSPECTIVE_CAMEREA_H_
