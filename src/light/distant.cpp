#include "distant.h"
#include <sampler/sampling.h>
#include <util/strutil.h>
#include <util/param.h>
ORION_NAMESPACE_BEGIN
void DistantLight::preprocess(const Scene & scene)
{
	scene.worldbound().boundingSphere(&worldCenter, &worldRadius);
}

Spectrum DistantLight::power() const
{
	return L * pi * worldRadius * worldRadius;
}

Spectrum DistantLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi,
	Float * pdf, ShadowTester * sdt, Point3f *samplePoint) const
{
	*wi = dir;
	*pdf = 1;
	Point3f p = isec.p + dir * (2 * worldRadius);
	if(samplePoint)
		*samplePoint = p;
	*sdt = ShadowTester(isec, Intersection(p));
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

std::shared_ptr<DistantLight> createDistantLight(const ParamSet & param)
{
	Spectrum L = parseSpectrum(param.getParam("L"));
	Point3f from = parsePoint3f(param.getParam("from"));
	Point3f to = parsePoint3f(param.getParam("to"));
	Vector3f dir = normalize(to - from);
	return std::shared_ptr<DistantLight>(new DistantLight(translate(Vector3f(from)), L, dir));
}

ORION_NAMESPACE_END
