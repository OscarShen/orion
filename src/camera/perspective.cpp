#include "perspective.h"
#include <util/strutil.h>
#include <sampler/sampler.h>
namespace orion {

	Ray PerspectiveCamera::generateRay(const Point2f &offset, StateSequence &rand) const
	{
		Point2f rand_offset = randomOffset(offset, rand(), rand());

		CHECK_INFO(film.get() != nullptr, "Note : no render target in camera!");
		Float width = (Float)film->getWidth();
		Float height = (Float)film->getHeight();
		Float aspectRatio = width / height;

		Float xx = (2 * (rand_offset.x + 0.5f) / (Float)width - 1) * aspectRatio * tan_half_fov;
		Float yy = (1 - 2 * (rand_offset.y + 0.5f) / (Float)height) * tan_half_fov;
		Float zz = -1.0f;

		Vector3f dir = normalize(Vector3f(xx, yy, zz));
		Ray before(Point3f(0), dir);
		Ray after = t(before);
		after.d = normalize(after.d);

		return after;
	}

	std::shared_ptr<Camera> createPerspectiveCamera(const ParamSet & param)
	{
		Point3f origin = parsePoint3f(param.getParam("origin"));
		Point3f lookat = parsePoint3f(param.getParam("lookat"));
		Vector3f up = parseVector3f(param.getParam("up"));
		Vector2i filmSize = parseVector2i(param.getParam("filmsize"));
		Float fov = parseFloat(param.getParam("fov"));

		auto film = std::make_shared<RenderTarget>(filmSize.x, filmSize.y);
		return std::shared_ptr<Camera>(new PerspectiveCamera(origin, lookat, up, fov, film));
	}

}
