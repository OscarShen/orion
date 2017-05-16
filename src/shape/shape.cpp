#include "shape.h"

namespace orion {

	Bounds3f Shape::worldBound() const
	{
		return (*local2world)(localBound());
	}
	Intersection Shape::sample(const Intersection & isec, const Point2f & rnd, Float * pdf) const
	{
		Intersection in = sample(rnd, pdf);
		Vector3f wi = in.pHit - isec.pHit;
		if (wi.lengthSquared() == 0) // if pdf = 0, means no contribution, not infinity!
			*pdf = 0;
		else {
			wi = normalize(wi);
			// solid angle measure
			*pdf *= (isec.pHit - in.pHit).lengthSquared() / absDot(in.n, -wi);
			if (std::isinf(*pdf)) *pdf = 0;
		}
		return in;
	}

	Float Shape::pdf(const Intersection & isec, const Vector3f & wi) const
	{
		Ray ray = isec.spawnRay(wi);
		Intersection in;
		if (!intersect(ray, &in)) return 0;

		Float pdf = (isec.pHit - in.pHit).lengthSquared() / (absDot(in.n, -wi) * area());
		if (std::isinf(pdf)) pdf = 0;
		return pdf;
	}

}
