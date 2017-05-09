#include "thinlens.h"
#include <sampler/sampler.h>
#include <util/strutil.h>
namespace orion {
	Ray ThinLensCamera::generateRay(const Point2f & offset, StateSequence & rand) const
	{
		Point2f rand_offset = randomOffset(offset, rand(), rand());

		CHECK_INFO(film.get() != nullptr, "Note : no render target in camera!");
		Float width = (Float)film->getWidth();
		Float height = (Float)film->getHeight();
		Float aspectRatio = width / height;

		Float xx = (2 * (rand_offset.x + 0.5f) / (Float)width - 1) * aspectRatio * tan_half_fov;
		Float yy = (1 - 2 * (rand_offset.y + 0.5f) / (Float)height) * tan_half_fov;
		Float zz = -1.0f;

		// normal ray
		Vector3f dir = normalize(Vector3f(xx, yy, zz));
		Ray before(Point3f(0), dir);

		if (lensRadius > 0) { // if has aperture
			Point3f target = before(focalDistance);

			Vector2f uv = sampleLens(rand.next2());
			before.o = Point3f(uv.x * lensRadius, uv.y * lensRadius, 0);
			before.d = normalize(target - before.o);
		}

		Ray after = t(before);

		return after;
	}
	Vector2f ThinLensCamera::sampleLens(const Vector2f & uv) const
	{
		Float r = std::sqrt(uv[0]);
		Float theta = pi * 2 * uv[1];
		return Vector2f(std::cos(theta), std::sin(theta)) * r;
	}
	std::shared_ptr<ThinLensCamera> createThinLensCamera(const ParamSet & param)
	{
		Point3f origin = parsePoint3f(param.getParam("origin"));
		Point3f lookat = parsePoint3f(param.getParam("lookat"));
		Vector3f up = parseVector3f(param.getParam("up"));
		Vector2i filmSize = parseVector2i(param.getParam("filmsize"));
		Float fov = parseFloat(param.getParam("fov"));

		auto film = std::make_shared<RenderTarget>(filmSize.x, filmSize.y);
		Float lensRadius = parseFloat(param.getParam("lensRadius"));
		Float focalDistance = parseFloat(param.getParam("focalDistance"));
		std::cout << "lensRadius : " << lensRadius << " focalDistance : " << focalDistance << std::endl;
		return std::shared_ptr<ThinLensCamera>(new ThinLensCamera(origin, lookat, up, fov, film, focalDistance, lensRadius));
	}
}