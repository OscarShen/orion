#include "distant.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN
void DistantLight::preprocess(const Scene & scene)
{
	scene.worldbound().boundingSphere(&worldCenter, &worldRadius);
}

Spectrum DistantLight::power() const
{
	return L * pi * worldRadius * worldRadius;
}

Spectrum DistantLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi, Float * pdf, ShadowTester * sdt) const
{
	*wi = dir;
	*pdf = 1;
	Point3f po = isec.p + dir * (2 * worldRadius);
	*sdt = ShadowTester(isec, Intersection(po));
	return L;
}

Spectrum DistantLight::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir) const
{
	Vector3f v1, v2;
	coordinateSystem(dir, &v1, &v2);
	Point2f cd = concentricSampleDisk(rand1);
	Point3f pDisk = worldCenter + worldRadius * (cd.x * v1 + cd.y * v2);

	// Set ray origin and direction for infinite light ray
	*ray = Ray(pDisk + worldRadius * dir, -dir);
	*nLight = (Normal3f)ray->d;
	*pdfPos = 1 / (pi * worldRadius * worldRadius);
	*pdfDir = 1;
	return L;
}

void DistantLight::pdf_Le(const Ray & ray, const Normal3f & n, Float * pdfPos, Float * pdfDir) const
{
	*pdfPos = 1 / (pi * worldRadius * worldRadius);
	*pdfDir = 0;
}

ORION_NAMESPACE_END