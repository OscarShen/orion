#include "diffuse.h"
#include <core/triangle.h>
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN

Spectrum DiffuseAreaLight::power() const
{
	return Le * area * pi;
}

Spectrum DiffuseAreaLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi, Float * pdf, ShadowTester * sdt, Point3f *samplePoint = nullptr) const
{
	Intersection in = triangle->sample(isec, rnd, pdf);
	if (samplePoint)
		*samplePoint = in.p;
	if (*pdf == 0 || (in.p - isec.p).lengthSquared() == 0) {
		*pdf = 0;
		return 0;
	}
	*wi = normalize(in.p - isec.p);
	*sdt = ShadowTester(isec, in);
	return L(in, -*wi);
}

Float DiffuseAreaLight::pdf_Li(const Intersection & isec, const Vector3f & wi, const Scene & scene) const
{
	return triangle->pdf(isec, wi, scene);
}

Spectrum DiffuseAreaLight::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir) const
{
	Intersection pShape = triangle->sample(rand1, pdfPos);
	*nLight = pShape.ng;

	Vector3f w = cosineSampleHemisphere(rand2);
	*pdfDir = cosineHemispherePdf(cosTheta(w));

	Vector3f z(pShape.ng), x, y;
	coordinateSystem(z, &x, &y);
	w = w.x * x + w.y * y + w.z * z;
	*ray = pShape.spawnRay(w);
	return L(pShape, w);
}

void DiffuseAreaLight::pdf_Le(const Ray & ray, const Normal3f & n, Float * pdfPos, Float * pdfDir) const
{
	*pdfPos = triangle->pdf();
	*pdfDir = cosineHemispherePdf(dot(n, ray.d));
}

Spectrum DiffuseAreaLight::L(const Intersection & intr, const Vector3f & w) const
{
	return dot(intr.ng, w) > 0 ? Le : 0;
}

ORION_NAMESPACE_END
