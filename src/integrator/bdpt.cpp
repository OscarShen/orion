#include "bdpt.h"
#include <core/scene.h>
#include <sampler/sampling.h>
#include <sampler/sampler.h>
#include <light/light.h>
#include <light/arealight.h>
#include <util/strutil.h>
namespace orion {

	void BDPathTracing::preprocess(const Scene & scene, Sampler & sampler)
	{
		int n = (int)scene.lights.size();
		CHECK_INFO(n > 0, "no light, you will get a black picture!");
		std::vector<Float> pdfs(n);
		for (int i = 0; i < n; ++i)
			pdfs[i] = scene.lights[i]->power().intensity();
		// We have only one light distribution which is power distribution.
		lightDistrib.reset(new Distribution1D(pdfs.data(), n));
	}
	Spectrum BDPathTracing::Li(const Ray & ray, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Sampler>& sampler, int depth) const
	{
		// select a light
		Float pdf;
		int lightIndex = lightDistrib->SampleDiscrete(sampler->next(), &pdf);
		const Light &light = *scene->lights[lightIndex];

		// sample a ray from light
		Float pdfPos, pdfDir, lightPdf;
		Ray lightRay;
		Normal3f n;
		Spectrum Le = light.sample_Le(sampler->next2(), sampler->next2(), &lightRay, &n, &pdfPos, &pdfDir);
		lightPdf = pdfPos * pdfDir;
		Le *= absDot(n, ray.d) / pdf;

		std::vector<Vertex> lightPath, cameraPath;
		// Sample all camera path, save intersection vertices.
		int cameraNumVert = generatePath(ray, *scene, *sampler, 1.0f, cameraPath);
		if (cameraNumVert == 0) return 0; // infinite area light
		// Sample light path
		int lightNumVert = generatePath(lightRay, *scene, *sampler, lightPdf, lightPath);

		// Add vertex on light
		Vertex lightInter;
		lightInter.p = lightRay.o;
		lightInter.n = n;
		lightInter.pdf = lightPdf;

		Spectrum directWt(1.0f);// direct weight
		Spectrum L;
		for (int i = 1; i <= cameraNumVert; ++i) {
			const Vertex &vert = cameraPath[i - 1];
			directWt /= vert.pdf;
			if (!directWt.isBlack()) {
				// estimate the light sample for all vertices
				L += directWt * estimateDirect(Ray(Point3f(0), -vert.wi), vert.isec, sampler->next2(),
					light, sampler->next2(), *scene, *sampler)
					* weightPath(cameraPath, i, lightPath, 0, lightInter) / pdf;
			}
			if (vert.pdf == 0)
				directWt = 0;
			directWt *= vert.bsdf->f(vert.wo, vert.wi) * absDot(vert.wo, vert.n);

			for (int j = 1; j <= lightNumVert; ++j) {
				L += Le * evaluatePath(*scene, cameraPath, i, lightPath, j) * weightPath(cameraPath, i, lightPath, j, lightInter);
			}
		}

		return L + cameraPath[0].isec.Le(-ray.d);
	}
	int BDPathTracing::generatePath(const Ray & ray, const Scene &scene, Sampler &sampler, Float basePdf, std::vector<Vertex>& path) const
	{
		Ray wi = ray;
		Ray wo;
		Float pdf = basePdf;
		int bounces = 0;
		while (bounces < maxDepth) {
			Vertex vert;
			if (!scene.intersect(wi, &vert.isec))
				break;
			vert.p = vert.isec.pHit;
			vert.n = vert.isec.n;
			vert.primitive = vert.isec.primitive;
			vert.wi = -wi.d;
			vert.pdf = pdf;
			vert.bsdf = vert.primitive->getMaterial()->getBSDF(&vert.isec);

			vert.bsdf->sample_f(&vert.wo, vert.wi, sampler.next2(), &pdf);
			path.push_back(vert);
			wi = vert.isec.spawnRay(vert.wo);
			++bounces;
		}
		// use up samples
		while (bounces < maxDepth) {
			sampler.next2();
			++bounces;
		}
		return (int)path.size();
	}
	Float BDPathTracing::weightPath(const std::vector<Vertex>& cameraPath, int cameraPathSize, const std::vector<Vertex>& lightPath, int lightPathSize, const Vertex & lightVertex) const
	{
		Float totalPdf = 0;
		int totalSize = cameraPathSize + lightPathSize;
		for (int i = 1; i <= totalSize; ++i) {
			totalPdf += pdfPath(cameraPath, i, lightPath, totalSize - i, lightVertex);
		}
		Float pdf = pdfPath(cameraPath, cameraPathSize, lightPath, lightPathSize, lightVertex);
		if (pdf == 0)
			return 0;
		return pdf / totalPdf;
	}
	Float BDPathTracing::pdfPath(const std::vector<Vertex>& cameraPath, int cameraPathSize, const std::vector<Vertex>& lightPath, int lightPathSize, const Vertex & lightVertex) const
	{
		if (cameraPathSize == 0)
			return 0;

		int cameraMaxSize = (int)cameraPath.size();
		int lightMaxSize = (int)lightPath.size();
		int totalSize = cameraPathSize + lightPathSize;

		const Vertex *preVert = &cameraPath[0];
		Float pdf = 1.0f;
		const Vertex *last = preVert; // record light vertex which is closest to light

		// Get all cosine term and pdf term in camera path and last camera vertex with all light vertices
		for (int i = 1; i < cameraPathSize; ++i) {
			if (i < cameraMaxSize) {
				preVert = &cameraPath[i];
				pdf *= absDot(preVert->wi, preVert->n) * preVert->pdf / (preVert->p - cameraPath[i - 1].p).lengthSquared();
				last = preVert;
			}
			else {
				if (lightPathSize == 0)
					break;
				const Vertex &vert = lightPath[totalSize - i - 1];
				Vector3f delta = preVert->p - vert.p;
				Vector3f wi(normalize(delta));
				pdf *= absDot(wi, vert.n) * preVert->bsdf->pdf(wi, preVert->wi) / delta.lengthSquared();
				preVert = &lightPath[totalSize - i - 1];
				last = preVert;
			}
		}

		// Get all cosine term and pdf term in light path and last light vertex with all camera vertices
		for (int i = 1; i < lightPathSize; ++i) {
			if (i < lightMaxSize) {
				preVert = &lightPath[i];
				pdf *= absDot(preVert->wi, preVert->n) * preVert->pdf / (preVert->p - lightPath[i - 1].p).lengthSquared();
			}
			else {
				const Vertex &vert = cameraPath[totalSize - i - 1];
				Vector3f delta = preVert->p - vert.p;
				Vector3f wi(normalize(delta));
				pdf *= absDot(wi, vert.n) * preVert->bsdf->pdf(wi, preVert->wi) / delta.lengthSquared();
				preVert = &cameraPath[totalSize - i - 1];
			}
		}

		if (lightMaxSize != 0)
			last = &lightPath[0];

		// compute pdf from light to last light vertex
		Vector3f wi(normalize(last->p - lightVertex.p));
		pdf *= absDot(wi, last->n) * lightVertex.pdf / (last->p - lightVertex.p).lengthSquared();
		return pdf;
	}
	Spectrum BDPathTracing::evaluatePath(const Scene &scene, const std::vector<Vertex>& cameraPath, int cameraPathSize, const std::vector<Vertex>& lightPath, int lightPathSize) const
	{

		Spectrum Li(1.0f);

		for (int i = 0; i < cameraPathSize - 1; ++i)
		{
			const Vertex& vert = cameraPath[i];
			Li *= vert.bsdf->f(vert.wo, vert.wi) * absDot(vert.wo, vert.n) / vert.pdf;
		}

		for (int i = 0; i < lightPathSize - 1; ++i)
		{
			const Vertex& vert = lightPath[i];
			Li *= vert.bsdf->f(vert.wi, vert.wo) * absDot(vert.wo, vert.n) / vert.pdf;
		}
		if (lightPath.empty())
			return 0;

		// connect respective last vertex
		const Vertex& cameraVert = cameraPath[cameraPathSize - 1];
		const Vertex& lightVert = lightPath[lightPathSize - 1];
		Vector3f delta = cameraVert.p - lightVert.p;
		Vector3f n_delta = normalize(delta);
		Spectrum l0 = cameraVert.bsdf->f(-n_delta, cameraVert.wi);
		float l1 = G(scene, cameraVert, lightVert);
		Spectrum l2 = lightVert.bsdf->f(lightVert.wi, n_delta);

		Li *= l0 * l1 * l2 / (cameraVert.pdf * lightVert.pdf);
		return Li;
	}
	Float BDPathTracing::G(const Scene &scene, const Vertex & v0, const Vertex & v1) const
	{
		Vector3f delta = v0.p - v1.p;
		Vector3f nDelta = normalize(delta);

		Float g = absDot(v0.n, -nDelta) * absDot(v1.n, nDelta) / delta.lengthSquared();
		if (g == 0)
			return 0;
		ShadowTester st(Intersection(v0.p), Intersection(v1.p));
		if (!st.unoccluded(scene))
			return 0;
		return g;
	}
	std::shared_ptr<BDPathTracing> createBDPathTracingIntegrator(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Sampler>& sampler, const ParamSet & param)
	{
		int maxDepth = parseInt(param.getParam("maxDepth"));
		return std::shared_ptr<BDPathTracing>(new BDPathTracing(camera, sampler, maxDepth));
	}
}
