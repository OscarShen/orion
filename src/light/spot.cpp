#include "spot.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN
Spectrum SpotLight::power() const
{
	return I * 2 * pi * (1 - .5f * (cosFalloffStart + cosTotalWidth));
}
Float SpotLight::falloff(const Vector3f & w) const
{
	Vector3f wl = normalize(world2local(w));
	Float cosTheta = wl.z;
	if (cosTheta < cosTotalWidth) return 0;
	if (cosTheta > cosFalloffStart) return 1;
	Float delta =
		(cosTheta - cosTotalWidth) / (cosFalloffStart - cosTotalWidth);
	return (delta * delta) * (delta * delta);
}
Spectrum SpotLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi, Float * pdf, ShadowTester * sdt) const
{
	*wi = normalize(p - isec.p);
	*pdf = 1.f;
	*sdt = ShadowTester(isec, Intersection(p));
	return I * falloff(-*wi) / (p - isec.p).lengthSquared();
}
Spectrum SpotLight::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir) const
{
	Vector3f w = uniformSampleCone(rand1, cosTotalWidth);
	*ray = Ray(p, local2world(w));
	*nLight = (Normal3f)ray->d;
	*pdfPos = 1;
	*pdfDir = uniformConePdf(cosTotalWidth);
	return I * falloff(ray->d);
}
void SpotLight::pdf_Le(const Ray & ray, const Normal3f & n, Float * pdfPos, Float * pdfDir) const
{
	*pdfPos = 0;
	*pdfDir = (cosTheta(world2local(ray.d)) >= cosTotalWidth)
		? uniformConePdf(cosTotalWidth) : 0;
}
ORION_NAMESPACE_END
