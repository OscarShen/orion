#include "light.h"
#include <core/scene.h>
#include <math/linalg.h>
#include <common/paramset.h>
#include <sampler/sampling.h>
#include <util/strutil.h>
namespace orion {

	Spectrum PointLight::sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const
	{
		Vector3f v = p - isec.pHit;
		*wi = normalize(v);
		*pdf = 1.0f;
		*sdt = ShadowTester(isec, Intersection(p));
		return I / v.lengthSquared();
	}
	Spectrum PointLight::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir)const
	{
		*ray = Ray(p, uniformSampleSphere(rand1));
		*nLight = (Normal3f)ray->d;
		*pdfPos = 1;
		*pdfDir = inv4pi;
		return I;
	}
	void PointLight::pdf_Le(const Ray & ray, const Normal3f & n, Float * pdfPos, Float * pdfDir) const
	{
		*pdfPos = 0;
		*pdfDir = inv4pi;
	}
	std::shared_ptr<PointLight> createPointLight(const ParamSet & param)
	{
		Spectrum I = parseSpectrum(param.getParam("intensity"));
		Point3f p = parsePoint3f(param.getParam("from"));
		Transform l2w = translate(Vector3f(p));
		return std::shared_ptr<PointLight>(new PointLight(l2w, I));
	}
	std::shared_ptr<SpotLight> createSpotLight(const ParamSet & param)
	{
		Spectrum I = parseSpectrum(param.getParam("intensity"));
		Float coneangle = parseFloat(param.getParam("angle"));
		Float conedelta = parseFloat(param.getParam("delta"));
		Point3f from = parsePoint3f(param.getParam("from"));
		Point3f to = parsePoint3f(param.getParam("to"));
		Vector3f dir = normalize(to - from);
		Vector3f z, x;
		coordinateSystem(dir, &z, &x);
		Transform dirToY(Matrix4f(x.x, x.y, x.z, 0.0f, dir.x, dir.y, dir.z, 0.0f, z.x, z.y, z.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		Transform l2w = translate(Vector3f(from)) * inverse(dirToY);
		return std::shared_ptr<SpotLight>(new SpotLight(l2w, I, coneangle, coneangle - conedelta));
	}
	std::shared_ptr<DistantLight> createDistantLight(const ParamSet & param)
	{
		Spectrum L = parseSpectrum(param.getParam("L"));
		Point3f from = parsePoint3f(param.getParam("from"));
		Point3f to = parsePoint3f(param.getParam("to"));
		Vector3f dir = normalize(to - from);
		return std::shared_ptr<DistantLight>(new DistantLight(translate(Vector3f(from)), L, dir));
	}
	SpotLight::SpotLight(const Transform & light2world, const Spectrum & I, Float totalWidth, Float falloffStart)
		: Light((int)LightType::DeltaPosition, light2world), I(I), p(light2world(Point3f(0))),
		cosTotalWidth(std::cos(radians(totalWidth))),
		cosFalloffStart(std::cos(radians(cosFalloffStart))) {}
	Spectrum SpotLight::sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const
	{
		Vector3f v = p - isec.pHit;
		*wi = normalize(v);
		*pdf = 1.0f;
		*sdt = ShadowTester(isec, Intersection(p));
		return I * falloff(-*wi) / v.lengthSquared();
	}
	Float SpotLight::falloff(const Vector3f & w) const
	{
		Vector3f wl = normalize(world2local(w));
		Float cosTh = cosTheta(wl);
		if (cosTh < cosTotalWidth) return 0;
		if (cosTh > cosFalloffStart) return 1;
		// compute falloff inside spotlight cone
		Float delta = (cosTh - cosTotalWidth) / (cosFalloffStart - cosTotalWidth);
		return delta * delta * delta * delta;
	}
	Spectrum SpotLight::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir)const
	{
		Vector3f w = uniformSampleCone(rand1, cosTotalWidth);
		*ray = Ray(p, local2world(w));
		*nLight = (Normal3f)ray->d;
		*pdfPos = 1;
		*pdfDir = uniformConePdf(cosTotalWidth);
		return I * falloff(ray->d);
	}
	void SpotLight::pdf_Le(const Ray &ray, const Normal3f &n, Float * pdfPos, Float * pdfDir) const
	{
		*pdfPos = 0;
		*pdfDir = (cosTheta(world2local(ray.d)) >= cosTotalWidth) ?
			uniformConePdf(cosTotalWidth) : 0;
	}
	void DistantLight::preprocess(const Scene & scene)
	{
		scene.worldBound().boundingSphere(&worldCenter, &worldRadius);
	}
	Spectrum DistantLight::sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi, Float *pdf, ShadowTester *sdt) const
	{
		*wi = -dir;
		*pdf = 1;
		*sdt = ShadowTester(isec, Intersection(isec.pHit + 1e5f * -dir));
		return L;
	}
	Spectrum DistantLight::power() const
	{
		return L * pi * worldRadius * worldRadius;
	}
	Spectrum DistantLight::sample_Le(const Point2f & rand1, const Point2f & rand2, Ray * ray, Normal3f * nLight, Float * pdfPos, Float * pdfDir)const
	{
		Vector3f x, z;
		coordinateSystem(dir, &z, &x);
		Point2f cd = concentricSampleDisk(rand1);
		Point3f p = worldCenter + worldRadius * (cd[0] * x + cd[1] * z);

		*ray = Ray(p + worldRadius * -dir, dir);
		*nLight = (Normal3f)ray->d;
		*pdfDir = 1;
		*pdfPos = 1 / (pi * worldRadius * worldRadius);
		return L;
	}
	void DistantLight::pdf_Le(const Ray &, const Normal3f &, Float * pdfPos, Float * pdfDir) const
	{
		*pdfPos = 1 / (pi * worldRadius * worldRadius);
		*pdfDir = 0;
	}
	bool ShadowTester::unoccluded(const std::shared_ptr<Scene> & scene) const
	{
		Intersection isec;
		return !scene->intersect(p0.spawnRay(p1), &isec); // TODO : add a fast calculation
	}
	bool ShadowTester::unoccluded(const Scene & scene) const
	{
		Intersection isec;
		return !scene.intersect(p0.spawnRay(p1), &isec); // TODO : add a fast calculation
	}
}
