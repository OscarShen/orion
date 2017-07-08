#include "sampling.h"

ORION_NAMESPACE_BEGIN

Vector3f uniformSampleSphere(const Point2f & rand)
{
	Float z = 1 - 2 * rand[0];
	Float r = std::sqrt(std::max((Float)0, (Float)1 - z * z));
	Float phi = 2 * pi * rand[1];
	return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}


Point2f uniformSampleTriangle(const Point2f & rand)
{
	Float su0 = std::sqrt(rand[0]);
	return Point2f(1 - su0, rand[1] * su0);
}

Vector3f uniformSampleCone(const Point2f & rand, Float cosThetaMax)
{
	Float cosTheta = ((Float)1 - rand[0]) + rand[0] * cosThetaMax;
	Float sinTheta = std::sqrt((Float)1 - cosTheta * cosTheta);
	Float phi = rand[1] * 2 * pi;
	return Vector3f(std::cos(phi) * sinTheta, std::sin(phi) * sinTheta,
		cosTheta);
}

Point2f uniformSampleDisk(const Point2f &u) {
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

Vector3f cosineSampleHemisphere(const Point2f & rand)
{
	Point2f sample = uniformSampleDisk(rand);
	Float z = std::sqrt(std::max(0.0f, 1 - sample[0] * sample[0] - sample[1] * sample[1]));
	return Vector3f(sample[0], sample[1], z);
}

Distribution2D::Distribution2D(const Float * func, int nu, int nv)
{
	pConditionalV.reserve(nv);
	for (int v = 0; v < nv; ++v) {
		// Compute conditional sampling distribution for $\tilde{v}$
		pConditionalV.emplace_back(new Distribution1D(&func[v * nu], nu));
	}
	// Compute marginal sampling distribution $p[\tilde{v}]$
	std::vector<Float> marginalFunc;
	marginalFunc.reserve(nv);
	for (int v = 0; v < nv; ++v)
		marginalFunc.push_back(pConditionalV[v]->funcInt);
	pMarginal.reset(new Distribution1D(&marginalFunc[0], nv));
}

ORION_NAMESPACE_END
