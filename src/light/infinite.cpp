#include "infinite.h"
#include <sampler/sampling.h>
ORION_NAMESPACE_BEGIN

InfiniteAreaLight::InfiniteAreaLight(const Transform & light2world, const Spectrum & power, int nSamples, const std::string & path)
	: Light((int)LightType::Infinite, light2world, nSamples)
{
	auto memory = loadImage(path);
	Lmap = std::make_unique<ImageTexture>(memory);

	int width = 2 * Lmap->getWidth(), height = 2 * Lmap->getHeight();
	std::unique_ptr<Float[]> img(new Float[width * height]);
	float fwidth = 0.5f / std::min(width, height);
	for (int j = 0; j < height; ++j) {
		Float v = (j + 0.5f) / (Float)height;
		Float sinTheta = std::sin(pi * v);
		for (int i = 0; i < width; ++i) {
			//Float u = (i + 0.5f) / (Float)width;
			Spectrum s = Lmap->sample(i, j);
			img[i + j * width] = s.intensity() * sinTheta;
			aveSpectrum += s;
		}
	}
	aveSpectrum /= width * height;
	distribution.reset(new Distribution2D(img.get(), width, height));
}

Spectrum InfiniteAreaLight::Le(const Ray & ray) const
{
	Vector3f w = normalize(world2local(ray.d));
	Float u = sphericalPhi(w), v = sphericalTheta(w);
	return Lmap->sample(u, v);
}

Spectrum InfiniteAreaLight::power() const
{
	return pi * worldRadius * worldRadius * aveSpectrum;
}
Spectrum InfiniteAreaLight::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi, Float * pdf, ShadowTester * sdt, Point3f * samplePoint) const
{
	Float mapPdf;
	Point2f uv = distribution->SampleContinuous(rnd, &mapPdf);
	if (mapPdf == 0)
		return 0;

	Float theta = uv[1] * pi, phi = uv[0] * 2 * pi;
	Float cosTheta = std::cos(theta), sinTheta = std::sin(theta);
	Float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
	*wi = local2world(Vector3f(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta));

	*pdf = mapPdf / (2 * pi * pi * sinTheta);
	if (sinTheta == 0)
		*pdf = 0;
	*sdt = ShadowTester(isec, Intersection(isec.p + *wi * (2 * worldRadius)));
	return Lmap->sample(uv.x, uv.y);
}
Float InfiniteAreaLight::pdf_Li(const Intersection & isec, const Vector3f & wi, const Scene & scene) const
{
	Vector3f swi = world2local(wi);
	Float theta = sphericalTheta(swi), phi = sphericalPhi(swi);
	Float sinTh = std::sin(theta);
	if (sinTheta == 0)
		return 0;
	return distribution->Pdf(Point2f(phi * inv2pi, theta * inv2pi)) /
		(2 * pi * pi * sinTh);
}
Spectrum InfiniteAreaLight::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir) const
{
	Point2f u = rand1;

	Float mapPdf;
	Point2f uv = distribution->SampleContinuous(u, &mapPdf);
	if (mapPdf == 0)
		return 0;
	Float theta = uv[1] * pi, phi = uv[0] * 2.f * pi;
	Float cosTheta = std::cos(theta), sinTheta = std::sin(theta);
	Float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
	Vector3f d = -local2world(Vector3f(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta));
	*nLight = (Normal3f)d;

	Vector3f v1, v2;
	coordinateSystem(-d, &v1, &v2);
	Point2f cd = concentricSampleDisk(rand2);
	Point3f pDisk = worldCenter + worldRadius * (cd.x * v1 + cd.y * v2);
	*ray = Ray(pDisk + worldRadius * -d, d);

	*pdfDir = sinTheta == 0 ? 0 : mapPdf / (2 * pi * pi * sinTheta);
	*pdfPos = 1 / (pi * worldRadius * worldRadius);
	return Lmap->sample(uv[0], uv[1]);
}
void InfiniteAreaLight::pdf_Le(const Ray & ray, const Normal3f & n, Float * pdfPos, Float * pdfDir) const
{
	Vector3f d = -world2local(ray.d);
	Float theta = sphericalTheta(d), phi = sphericalPhi(d);
	Point2f uv(phi * inv2pi, theta * invpi);
	Float mapPdf = distribution->Pdf(uv);
	*pdfDir = mapPdf / (2 * pi * pi * std::sin(theta));
	*pdfPos = 1 / (pi * worldRadius * worldRadius);
}
ORION_NAMESPACE_END
