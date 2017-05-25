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
		std::shared_ptr<RenderTarget> film;

	public:
		Camera() {}
		Camera(const std::shared_ptr<RenderTarget> &film)
			: film(film) {}
		virtual ~Camera() {}

		virtual Ray generateRay(const Point2f &offset, const std::shared_ptr<Sampler> &sampler) const = 0;

		void setRenderTarget(const std::shared_ptr<RenderTarget> &film) { this->film = film; }
		std::shared_ptr<RenderTarget> getFilm() const { 
			CHECK_INFO(film.get() != nullptr, "Note : no render target in camera!");
			return film;
		}
	};
}

#endif // !ORION_CAMERA_H_
