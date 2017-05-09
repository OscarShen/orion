/************************************************************************ 
 * @description :  Camera base class
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-21 20:21:42
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CAMERA_H_
#define ORION_CAMERA_H_
#include <orion.h>
#include <core/geometry.h>
#include <texture/rendertarget.h>
namespace orion {

	class Camera
	{
	protected:
		Point3f orig;
		std::shared_ptr<RenderTarget> film;

	public:
		Camera() {}
		Camera(const Point3f &orig, const std::shared_ptr<RenderTarget> &film)
			:orig(orig), film(film) {}
		virtual ~Camera() {}

		virtual Ray generateRay(const Point2f &offset, StateSequence &rand) const = 0;

		void setRenderTarget(const std::shared_ptr<RenderTarget> &film) { this->film = film; }
		void setOrig(const Point3f &o) { orig = o; }
		Point3f getOrig() const { return orig; }
		std::shared_ptr<RenderTarget> getFilm() const { 
			CHECK_INFO(film.get() != nullptr, "Note : no render target in camera!");
			return film;
		}

	protected:
		Point2f randomOffset(const Point2f &offset, Float u, Float v) const {
			return Point2f(offset.x + u, offset.y + v);
		}
	};

}

#endif // !ORION_CAMERA_H_
