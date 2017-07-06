#include "point.h"
#include <sampler/sampling.h>
#include <util/param.h>
#include <util/strutil.h>
ORION_NAMESPACE_BEGIN
Spectrum PointLight::power() const
{
	return 4 * pi * I;
}
Spectrum PointLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi, 
	Float * pdf, ShadowTester * sdt, Point3f *samplePoint) const
{
	*wi = normalize(p - isec.p);
	*pdf = 1.0f;
	if(samplePoint)
		*samplePoint = p;
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

std::shared_ptr<PointLight> createPointLight(const ParamSet & param)
{
	Spectrum I = parseSpectrum(param.getParam("I"));
	Point3f p = parsePoint3f(param.getParam("from"));
	Transform l2w = translate(Vector3f(p));
	return std::shared_ptr<PointLight>(new PointLight(l2w, I));
}

ORION_NAMESPACE_END
