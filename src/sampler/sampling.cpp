#include "sampling.h"

namespace orion {

	Point2f uniformSampleDisk(const Point2f & u)
	{
		Float r = std::sqrt(u[0]);
		Float theta = 2 * pi * u[1];
		return Point2f(r * std::cos(theta), r * std::sin(theta));
	}

	Point2f concentricSampleDisk(const Point2f & rand)
	{
		// Map uniform random numbers to $[-1,1]^2$
		Point2f uOffset = 2.f * rand - Vector2f(1.0f, 1.0f);

		// Handle degeneracy at the origin
		if (uOffset.x == 0 && uOffset.y == 0) return Point2f(0, 0);

		// Apply concentric mapping to point
		Float theta, r;
		if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
			r = uOffset.x;
			theta = piover4 * (uOffset.y / uOffset.x);
		}
		else {
			r = uOffset.y;
			theta = piover2 - piover4 * (uOffset.x / uOffset.y);
		}
		return Point2f(std::cos(theta), std::sin(theta)) * r;
	}

	Point2f uniformSampleTriangle(const Point2f & u)
	{
		Float su0 = std::sqrt(u[0]);
		return Point2f(1 - su0, u[1] * su0);
	}

}
