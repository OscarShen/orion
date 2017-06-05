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

	Vector3f uniformSampleSphere(const Point2f & rand)
	{
		Float y = 1 - 2 * rand[0];
		Float r = std::sqrt(std::max(0.0f, 1.0f - y * y));
		Float phi = 2 * pi * rand[1];
		return Vector3f(r * std::cos(phi), y, -r * std::sin(phi));
	}

	Vector3f uniformSampleCone(const Point2f & rand, Float cosThetaMax)
	{
		Float cosTheta = (1 - rand[0]) + rand[0] * cosThetaMax;
		Float sinTheta = std::sqrt(1 - cosTheta * cosTheta);
		Float phi = rand[1] * 2 * pi;
		return Vector3f(sinTheta * std::cos(phi), cosTheta, -sinTheta * std::sin(phi));
	}

	Vector3f cosineSampleHemisphere(const Point2f & rand)
	{
		Point2f sample = concentricSampleDisk(rand);
		Float y = std::sqrt(std::max(0.0f, 1 - sample[0] * sample[0] - sample[1] * sample[1]));
		return Vector3f(sample[0], y, sample[1]);
	}

}
