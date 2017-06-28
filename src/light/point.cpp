#include "point.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN
Spectrum PointLight::power() const
{
	return 4 * pi * I;
}
Spectrum PointLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi, Float * pdf, ShadowTester * sdt) const
{
	*wi = normalize(p - isec.p);
	*pdf = 1.0f;
	*sdt = ShadowTester(isec, Intersection(p));
	return I / (p - isec.p).lengthSquared();
}
Spectrum PointLight::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir) const
{
	*ray = Ray(p, uniformSampleSphere(rand1));
	*nLight = (Normal3f)ray->d;
	*pdfPos = 1;
	*pdfDir = uniformSpherePdf();
	return I;
}
void PointLight::pdf_Le(const Ray & ray, const Normal3f & n, Float * pdfPos, Float * pdfDir) const
{
	*pdfPos = 0;
	*pdfDir = uniformSpherePdf();
}
ORION_NAMESPACE_END
