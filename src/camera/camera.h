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
		Float aspectRatio;

	public:
		Camera() {}
		Camera(const std::shared_ptr<RenderTarget> &film)
			: film(film), aspectRatio((Float)film->getWidth() / film->getHeight()) {}
		virtual ~Camera() {}

		virtual Ray generateRay(const Point2f &offset, const std::shared_ptr<Sampler> &sampler) const = 0;

		void setRenderTarget(const std::shared_ptr<RenderTarget> &film) { this->film = film; }

		virtual Spectrum We(const Ray &ray, Point2f *pRaster2 = nullptr) const {
			CHECK_INFO(false, "no impl!");
			return 0;
		}
		virtual void Pdf_We(const Ray &ray, Float *pdfPos, Float *pdfDir) const {
			CHECK_INFO(false, "no impl!");
		}

		virtual Spectrum Sample_Wi(const Intersection &ref, const Point2f &u,
			Vector3f *wi, Float *pdf, Point2f *pRaster, ShadowTester *vis) const {
			CHECK_INFO(false, "no impl!");
			return 0;
		}

		std::shared_ptr<RenderTarget> getFilm() const { 
			CHECK_INFO(film.get() != nullptr, "Note : no render target in camera!");
			return film;
		}
	};
}

#endif // !ORION_CAMERA_H_
