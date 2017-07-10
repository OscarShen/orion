#include "envmap.h"
#include <sampler/sampling.h>
#include <util/param.h>
#include <util/strutil.h>
#include <util/envvariable.h>
ORION_NAMESPACE_BEGIN

Envmap::Envmap(const Transform & light2world, const Spectrum & scale, int nSamples, const std::string & path)
	: Light((int)LightType::Infinite, light2world, nSamples)
{
	auto memory = loadImage(path);
	int width = memory->width, height = memory->height;
	auto a = std::make_shared<ImageTexture>(memory);
	Spectrum *data = memory->data;
	for (int i = 0; i < width * height; ++i)
		data[i] *= scale;
	Lmap = std::make_unique<ImageTexture>(memory);
	
	std::unique_ptr<Float[]> img(new Float[width * height]);
	for (int j = 0; j < height; ++j) {
		Float v = (j + 0.5f) / (Float)height;
		Float sinTheta = std::sin(pi * v);
		for (int i = 0; i < width; ++i) {
			Float u = (i + 0.5f) / (Float)width;
			Spectrum s = Lmap->sample(i, j);
			img[i + j * width] = s.intensity() * sinTheta;
			aveSpectrum += s;
		}
	}
	aveSpectrum /= width * height;
	distribution.reset(new Distribution2D(img.get(), width, height));
}

Spectrum Envmap::Le(const Ray & ray) const
{
	Vector3f w = normalize(world2local(ray.d));
	Float u = sphericalPhi(w) * inv2pi, v = sphericalTheta(w) * invpi;
	return Lmap->sample(u, v);
}

Spectrum Envmap::power() const
{
	return pi * worldRadius * worldRadius * aveSpectrum;
}
Spectrum Envmap::sample_Li(const Intersection & isec, const Point2f & rnd, Vector3f * wi, Float * pdf, ShadowTester * sdt, Point3f * samplePoint) const
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
Float Envmap::pdf_Li(const Intersection & isec, const Vector3f & wi, const Scene & scene) const
{
	Vector3f swi = world2local(wi);
	Float theta = sphericalTheta(swi), phi = sphericalPhi(swi);
	Float sinTh = std::sin(theta);
	if (sinTheta == 0)
		return 0;
	return distribution->Pdf(Point2f(phi * inv2pi, theta * inv2pi)) /
		(2 * pi * pi * sinTh);
}
Spectrum Envmap::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir) const
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
void Envmap::pdf_Le(const Ray & ray, const Normal3f & n, Float * pdfPos, Float * pdfDir) const
{
	Vector3f d = -world2local(ray.d);
	Float theta = sphericalTheta(d), phi = sphericalPhi(d);
	Point2f uv(phi * inv2pi, theta * invpi);
	Float mapPdf = distribution->Pdf(uv);
	*pdfDir = mapPdf / (2 * pi * pi * std::sin(theta));
	*pdfPos = 1 / (pi * worldRadius * worldRadius);
}

std::shared_ptr<Envmap> createEnvMap(const Transform &transform, const ParamSet & param)
{
	Spectrum scale = param.hasParam("scale") ? parseSpectrum(param.getParam("scale")) : Spectrum(1.0f);
	std::string name = param.getParam("name");
	std::string path = getResPath() + '/' + name;
	Vector3f worldUp = normalize(parseVector3f(param.getParam("worldUp")));
	Vector3f envmapUp(0, 0, 1);

	Float cosAngle = dot(worldUp, envmapUp);
	Transform t;
	if (cosAngle < 0.99 && cosAngle > -0.99) {
		Float angle = acos(dot(worldUp, envmapUp));
		Vector3f axis = cross(envmapUp, worldUp);
		t = rotate(degrees(angle), axis);
	}

	int nSamples = parseInt(param.getParam("nLightSamples"));
	return std::make_shared<Envmap>(t * transform, scale, nSamples, path);
}

ORION_NAMESPACE_END
