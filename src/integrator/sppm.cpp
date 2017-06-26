#include "sppm.h"
#include <common/parallel.h>
#include <common/paramset.h>
#include <core/spectrum.h>
#include <core/scene.h>
#include <sampler/sampler.h>
#include <sampler/sampling.h>
#include <light/light.h>
#include <util/strutil.h>
namespace orion {

	struct SPPMPixel
	{
		SPPMPixel() {}
		Float radius = 0;
		Spectrum Ld;
		AtomicFloat phi[3];
		std::atomic<int> M = 0;
		Float N = 0;
		Spectrum tau;

		struct VisiblePoint {
			Point3f p;
			Vector3f wo;
			std::shared_ptr<BSDF> bsdf;
			Spectrum beta;
		} vp;
	};

	struct SPPMPixelListNode
	{
		SPPMPixel *pixel;
		SPPMPixelListNode *next;
	};

	static bool toGrid(const Point3f &p, const Bounds3f &bounds,
		const int gridRes[3], Point3i *pi) {
		bool inBounds = true;
		Vector3f pg = bounds.offset(p);
		for (int i = 0; i < 3; ++i) {
			(*pi)[i] = (int)(gridRes[i] * pg[i]);
			inBounds &= ((*pi)[i] >= 0 && (*pi)[i] < gridRes[i]);
			(*pi)[i] = clamp((*pi)[i], 0, gridRes[i] - 1);
		}
		return inBounds;
	}

	inline unsigned int hash(const Point3i &p, int hashSize) {
		return (unsigned int)((p.x * 73856093) ^ (p.y * 19349663) ^
			(p.z * 83492791)) % hashSize;
	}

	SPPMIntegrator::SPPMIntegrator(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Sampler>& sampler, int maxDepth, int nIterations, int photonPerIteration, Float initialSerachRadius)
		: Integrator(camera, sampler), maxDepth(maxDepth), initSearchRadius(initialSerachRadius),
		numIter(nIterations), numPhotonsIter(photonPerIteration)
	{
	}

	void SPPMIntegrator::preprocess(const Scene & scene, Sampler & sampler)
	{
		int n = (int)scene.lights.size();
		CHECK_INFO(n > 0, "no light, you will get a black picture!");
		std::vector<Float> pdfs(n);
		for (int i = 0; i < n; ++i)
			pdfs[i] = scene.lights[i]->power().intensity();
		// We have only one light distribution which is power distribution.
		lightDistrib.reset(new Distribution1D(pdfs.data(), n));
	}

	void SPPMIntegrator::render(const Scene & scene)
	{
		std::shared_ptr<RenderTarget> film = camera->getFilm();
		int filmWidth = film->getWidth(), filmHeight = film->getHeight();
		int nPixels = filmWidth * filmHeight;
		std::unique_ptr<SPPMPixel[]> pixels(new SPPMPixel[nPixels]);
		for (int i = 0; i < nPixels; ++i)
			pixels[i].radius = initSearchRadius;

		Float invSqrtSPP(1.f / std::sqrt((Float)numIter));
		for (int iter = 0; iter < numIter; ++iter) {
			//std::cout << iter << std::endl;
			// Accumulating visible points
			{
				#pragma omp parallel for
				for (int j = 0; j < filmHeight; ++j) {
					for (int i = 0; i < filmWidth; ++i) {
						std::shared_ptr<Sampler> sampler = this->sampler->clone(iter * nPixels + j * filmWidth + i);
						Ray ray = camera->generateRay(Point2f((Float)i, (Float)j), sampler);
						Spectrum beta(1.0f);
						SPPMPixel &pixel = pixels[j * filmWidth + i];
						bool specularBounce = false;
						for (int depth = 0; depth < maxDepth; ++depth) {
							Intersection isec;
							if (!scene.intersect(ray, &isec)) {
								// infinity light
								break;
							}
							std::shared_ptr<BSDF> bsdf = isec.primitive->getMaterial()->getBSDF(&isec);

							// accumulate direct illumination at SPPM camera ray
							Vector3f wo = -ray.d;
							if (depth == 0 || specularBounce)
								pixel.Ld += beta * isec.Le(wo);
							pixel.Ld += beta * uniformSampleOneLight(ray, isec, scene, *sampler);

							bool isDiffuse = bsdf->numComponents(BxDF_TYPE(BxDF_DIFFUSE | BxDF_REFLECTION | BxDF_TRANSMISSION)) > 0;
							bool isGlossy = bsdf->numComponents(BxDF_TYPE(BxDF_GLOSSY | BxDF_REFLECTION | BxDF_TRANSMISSION)) > 0;
							if (isDiffuse || (isGlossy && depth == maxDepth - 1)) {
								pixel.vp = { isec.pHit, wo, bsdf, beta };
								break;
							}
							if (depth < maxDepth - 1) {
								Float pdf;
								Vector3f wi;
								BxDF_TYPE type;
								Spectrum f = bsdf->sample_f(&wi, wo, sampler->next2(), &pdf, BxDF_ALL, &type);
								if (pdf == 0 || f.isBlack())
									break;
								specularBounce = (type & BxDF_SPECULAR) != 0;
								beta *= f * absDot(wi, isec.n) / pdf;
								if (beta.intensity() < 0.25f) {
									Float continueProb = std::min((Float)1.0f, beta.intensity());
									if (orion::rand() > continueProb)
										break;
									beta /= continueProb;
								}
								ray = isec.spawnRay(wi);
							}
						}
					}
				} 
			} // Accumulating visible points


			// Visible point grid construction
			int gridRes[3];
			Bounds3f gridBounds;
			const int hashSize = nPixels;
			std::vector<std::atomic<SPPMPixelListNode *>> grid(hashSize);
			std::vector<SPPMPixelListNode*> gridNode;

			Float maxRadius = 0;
			for (int i = 0; i < nPixels; ++i) {
				const SPPMPixel &pixel = pixels[i];
				if (pixel.vp.beta.isBlack())
					continue;	// no photon in grid or path terminated before intersecting a diffuse surface
				Bounds3f vpBounds = expand(Bounds3f(pixel.vp.p), pixel.radius);
				gridBounds = Union(gridBounds, vpBounds);
				maxRadius = std::max(maxRadius, pixel.radius);
			}

			Vector3f diag = gridBounds.diagonal();
			Float maxDiag = maxComponent(diag);
			int baseGridRes = (int)(maxDiag / maxRadius);
			CHECK_INFO(baseGridRes > 0, "Grid of SPPM less than zero!");
			for (int i = 0; i < 3; ++i)
				gridRes[i] = std::max((int)(baseGridRes * diag[i] / maxDiag), 1);
			// Add visible points to SPPM grid
			#pragma omp parallel for
			for (int pixelIndex = 0; pixelIndex < nPixels; ++pixelIndex) {
				SPPMPixel &pixel = pixels[pixelIndex];
				if (!pixel.vp.beta.isBlack()) {
					Float radius = pixel.radius;
					Point3i pMin, pMax;
					toGrid(pixel.vp.p - Vector3f(radius), gridBounds, gridRes, &pMin);
					toGrid(pixel.vp.p + Vector3f(radius), gridBounds, gridRes, &pMax);
					for (int z = pMin.z; z <= pMax.z; ++z)
						for (int y = pMin.y; y <= pMax.y; ++y)
							for (int x = pMin.x; x <= pMax.x; ++x) {
								int h = hash(Point3i(x, y, z), hashSize);
								SPPMPixelListNode *node = new SPPMPixelListNode();
								#pragma omp critical
								gridNode.push_back(node);
								node->pixel = &pixel;

								node->next = grid[h];
								while (grid[h].compare_exchange_weak(node->next, node) == false)
									;
							}
				}
			}

			// Accumulateing photon contributions
			{
				#pragma omp parallel for
				for (int photonIndex = 0; photonIndex < numPhotonsIter; ++photonIndex) {
					int64_t seed = numIter * nPixels + (uint64_t)iter * (uint64_t)numPhotonsIter + photonIndex;
					std::shared_ptr<Sampler> s = this->sampler->clone(seed);

					Float lightPdf;
					int lightNum = lightDistrib->SampleDiscrete(s->next(), &lightPdf);
					const std::shared_ptr<Light> &light = scene.lights[lightNum];

					Point2f lightSample0 = s->next2(), lightSample1 = s->next2();
					//lightSample0 = Point2f(0);

					Ray photonRay;
					Normal3f nLight;
					Float pdfPos, pdfDir;
					Spectrum Le = light->sample_Le(lightSample0, lightSample1, &photonRay, &nLight, &pdfPos, &pdfDir);

					if (pdfPos == 0 || pdfDir == 0 || Le.isBlack())
						continue;
					Spectrum beta = absDot(nLight, photonRay.d) * Le / (lightPdf * pdfPos * pdfDir);
					if (beta.isBlack())
						continue;

					Intersection isec;
					for (int depth = 0; depth < maxDepth; ++depth) {
						if (!scene.intersect(photonRay, &isec))
							break;
						if (depth > 0) {
							Point3i photonGridIndex;
							if (toGrid(isec.pHit, gridBounds, gridRes, &photonGridIndex)) {
								int h = hash(photonGridIndex, hashSize);
								for (SPPMPixelListNode *node = grid[h].load(std::memory_order_relaxed);
									node != nullptr; node = node->next) {
									SPPMPixel &pixel = *node->pixel;
									Float radius = pixel.radius;
									if ((pixel.vp.p - isec.pHit).lengthSquared() > radius * radius)
										continue;
									Vector3f wi = -photonRay.d;
									Spectrum phi = beta * pixel.vp.bsdf->f(wi, pixel.vp.wo);
									pixel.phi[0].add(phi.r);
									pixel.phi[1].add(phi.g);
									pixel.phi[2].add(phi.b);
									++pixel.M;
								}
							}
						}

						std::shared_ptr<BSDF> photonBsdf = isec.primitive->getMaterial()->getBSDF(&isec, false);
						Vector3f wi, wo = -photonRay.d;
						Float pdf;
						BxDF_TYPE flags;

						Point2f bsdfSample = s->next2();
						Spectrum fr = photonBsdf->sample_f(&wi, wo, bsdfSample, &pdf, BxDF_ALL, &flags);
						if (fr.isBlack() || pdf == 0)
							break;
						Spectrum bnew = beta * fr * absDot(wi, isec.n) / pdf;

						Float q = std::max((Float)0, 1 - bnew.intensity() / beta.intensity());
						if (s->next() < q)
							break;
						beta = bnew / (1 - q);
						photonRay = isec.spawnRay(wi);
					}
				}
				for (auto node : gridNode)
					delete node;
			} // Accumulateing photon contributions

			// Update pixel values
			{
				#pragma omp parallel for
				for (int i = 0; i < nPixels; ++i) {
					SPPMPixel &p = pixels[i];
					if (p.M > 0) {
						Float gamma = 0.66666667f;
						Float Nnew = p.N + gamma * p.M;
						Float Rnew = p.radius * std::sqrt(Nnew / (p.N + p.M));
						Spectrum phi;
						phi.r = p.phi[0];
						phi.g = p.phi[1];
						phi.b = p.phi[2];
						p.tau = (p.tau + p.vp.beta * phi) * (Rnew * Rnew) /
							(p.radius * p.radius);
						p.N = Nnew;
						p.radius = Rnew;
						p.M = 0;
						for (int j = 0; j < 3; ++j)
							p.phi[j] = (Float)0;
					}
					p.vp.beta = 0;
					p.vp.bsdf = nullptr;
				}
			} // Update pixel values
		}
		uint64_t Np = (uint64_t)numIter * (uint64_t)numPhotonsIter;
		for (int j = 0; j < filmHeight; ++j)
			for (int i = 0; i < filmWidth; ++i) {
				const SPPMPixel &pixel = pixels[j * filmWidth + i];
				Spectrum L = pixel.Ld / numIter;
				L += pixel.tau / (Np * pi * pixel.radius * pixel.radius);
				film->acumulate(i, j, L);
			}
	}
	std::shared_ptr<SPPMIntegrator> createSPPMIntegrator(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Sampler>& sampler, const ParamSet & param)
	{
		int nIterations = parseInt(param.getParam("numIterations"));
		int maxDepth = parseInt(param.getParam("maxDepth"));
		int photonsPerIter = parseInt(param.getParam("photonsPerIteration"));
		if (photonsPerIter < 0)
			photonsPerIter = camera->getFilm()->getHeight() * camera->getFilm()->getWidth();
		Float radius = parseFloat(param.getParam("radius"));
		return std::shared_ptr<SPPMIntegrator>(new SPPMIntegrator(camera, sampler, maxDepth, nIterations,
			photonsPerIter, radius));
	}
}
