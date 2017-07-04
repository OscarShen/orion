#include "perspective.h"
#include <util/strutil.h>
#include <sampler/sampler.h>
#include <sampler/sampling.h>
#include <light/light.h>
namespace orion {

	Ray PerspectiveCamera::generateRay(const Point2f &offset, const std::shared_ptr<Sampler> &sampler) const
	{
		Point2f rand_offset = offset + sampler->next2();

		CHECK_INFO(film.get() != nullptr, "Note : no render target in camera!");
		Float width = (Float)film->getWidth();
		Float height = (Float)film->getHeight();

		Float xx = (2 * rand_offset.x / (Float)width - 1) * aspectRatio * tan_half_fov;
		Float yy = (1 - 2 * rand_offset.y / (Float)height) * tan_half_fov;
		Float zz = -1.0f;

		Vector3f dir = normalize(Vector3f(xx, yy, zz));
		Ray before(Point3f(0), dir);

		if (lensRadius > 0) { // if has aperture
			Point3f target = before(focalDistance);

			Point2f uv = uniformSampleDisk(sampler->next2());
			before.o = Point3f(uv.x * lensRadius, uv.y * lensRadius, 0);
			before.d = normalize(target - before.o);
		}

		Ray after = t(before);
		return after;
	}

	Spectrum PerspectiveCamera::We(const Ray & ray, Point2f * pRaster2) const
	{
		Float costheta = dot(ray.d, t(Vector3f(0, 0, -1))); // default ray generation is from (0,0,0) to (0,0,-1)
		if (costheta <= 0)
			return 0;

		Point3f pFocus = ray((lensRadius > 0 ? focalDistance : 1) / costheta);

		Ray pRay = inverse(t)(ray);
		Float pRayT = pRay.d.z / (lensRadius > 0 ? -focalDistance : -1);
		Point3f pRaster = pRay(pRayT);
		*pRaster2 = Point2f(pRaster.x, pRaster.y);
		
		Float lensArea = lensRadius > 0 ? (pi * lensRadius * lensRadius) : 1;
		Float cos2theta = costheta * costheta;

		Float maxX = aspectRatio * tan_half_fov;
		Float maxY = tan_half_fov;
		if (pRaster.x < -maxX || pRaster.x > maxX || pRaster.y < -maxY || pRaster.y > maxY)
			return 0;

		Float rasterArea = 4 * maxX * maxY;
		return Spectrum(1 / (rasterArea * lensArea * cos2theta * cos2theta)); // PBRT.v3 p952
	}

	void PerspectiveCamera::Pdf_We(const Ray & ray, Float * pdfPos, Float * pdfDir) const
	{
		Float costheta = dot(ray.d, t(Vector3f(0, 0, -1))); // default ray generation is from (0,0,0) to (0,0,-1)
		if (costheta <= 0) {
			*pdfPos = *pdfDir = 0;
			return;
		}

		Point3f pFocus = ray((lensRadius > 0 ? focalDistance : 1) / costheta);

		Ray pRay = inverse(t)(ray);
		Float pRayT = pRay.d.z / (lensRadius > 0 ? -focalDistance : -1);
		Point3f pRaster = pRay(pRayT);

		Float lensArea = lensRadius > 0 ? (pi * lensRadius * lensRadius) : 1;
		Float cos2theta = costheta * costheta;

		Float maxX = aspectRatio * tan_half_fov;
		Float maxY = tan_half_fov;
		if (pRaster.x < -maxX || pRaster.x > maxX || pRaster.y < -maxY || pRaster.y > maxY) {
			*pdfPos = *pdfDir = 0;
			return;
		}

		Float rasterArea = 4 * maxX * maxY;
		*pdfPos = 1 / lensArea;
		*pdfDir = 1 / (rasterArea * costheta * costheta * costheta);
	}

	Spectrum PerspectiveCamera::Sample_Wi(const Intersection & ref, const Point2f & u, Vector3f * wi, Float * pdf, Point2f * pRaster, ShadowTester * vis) const
	{
		Point2f pLens = lensRadius * concentricSampleDisk(u);
		Point3f pLensWorld = t(Point3f(pLens.x, pLens.y, 0));
		Intersection lensIsec(pLensWorld);
		lensIsec.n = Normal3f(t(Vector3f(0, 0, -1)));

		*vis = ShadowTester(ref, lensIsec);
		*wi = lensIsec.pHit - ref.pHit;
		Float dist = wi->length();
		*wi /= dist;

		// pdf
		Float lensArea = lensRadius > 0 ? (pi * lensRadius * lensRadius) : 1;
		*pdf = (dist * dist) / (absDot(lensIsec.n, *wi) * lensArea);			// for dA
		return We(lensIsec.spawnRay(-*wi), pRaster);
	}

	std::shared_ptr<Camera> createPerspectiveCamera(const Transform &camera2world, const ParamSet & param)
	{
		Vector2i filmSize = parseVector2i(param.getParam("filmsize"));
		Float fov = parseFloat(param.getParam("fov"));

		auto film = std::make_shared<RenderTarget>(filmSize.x, filmSize.y);

		Float lensRadius = 0, focalDistance = 0;
		if (param.hasParam("lensRadius")) {
			lensRadius = parseFloat(param.getParam("lensRadius"));
			focalDistance = parseFloat(param.getParam("focalDistance"));
		}
		return std::shared_ptr<Camera>(new PerspectiveCamera(camera2world, fov, film,focalDistance, lensRadius));
	}
}
