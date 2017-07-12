#include "spot.h"
#include <sampler/sampling.h>
#include <util/param.h>
#include <util/strutil.h>
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
Spectrum SpotLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi,
	Float * pdf, ShadowTester * sdt, Point3f *samplePoint) const
{
	*wi = normalize(p - isec.p);
	*pdf = 1.f;
	if(samplePoint)
		*samplePoint = p;
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

std::shared_ptr<SpotLight> createSpotLight(const ParamSet & param)
{
	Spectrum I = parseSpectrum(param.getParam("intensity"));
	Float coneangle = parseFloat(param.getParam("angle"));
	Float conedelta = parseFloat(param.getParam("delta"));
	Point3f from = parsePoint3f(param.getParam("from"));
	Point3f to = parsePoint3f(param.getParam("to"));
	Vector3f dir = normalize(to - from);
	Vector3f x, y;
	coordinateSystem(dir, &x, &y);
	Transform dirToY(Matrix4f(x.x, x.y, x.z, 0.0f, y.x, y.y, y.z, 0.0f, dir.x, dir.y, dir.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	Transform l2w = translate(Vector3f(from)) * inverse(dirToY);
	return std::shared_ptr<SpotLight>(new SpotLight(l2w, I, coneangle, coneangle - conedelta));
}

ORION_NAMESPACE_END
