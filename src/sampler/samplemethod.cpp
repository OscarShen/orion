#include "samplemethod.h"

namespace orion {

	Point2f uniformSampleDisk(const Point2f & u)
	{
		Float r = std::sqrt(u[0]);
		Float theta = 2 * pi * u[1];
		return Point2f(r * std::cos(theta), r * std::sin(theta));
	}

	Point2f uniformSampleTriangle(const Point2f & u)
	{
		Float su0 = std::sqrt(u[0]);
		return Point2f(1 - su0, u[1] * su0);
	}

}
