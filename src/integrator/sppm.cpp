#include "sppm.h"
#include <core/camera.h>
#include <core/sampler.h>
#include <sampler/sampling.h>
#include <util/threading.h>
#include <util/strutil.h>
#include <util/param.h>
#include <util/renderinfo.h>
ORION_NAMESPACE_BEGIN

struct SPPMPixel {
	// SPPMPixel Public Methods
	SPPMPixel() : M(0) {}

	// SPPMPixel Public Data
	Float radius = 0;
	Spectrum Ld;
	struct VisiblePoint {
		// VisiblePoint Public Methods
		VisiblePoint() {}
		VisiblePoint(const Point3f &p, const Vector3f &wo, const std::shared_ptr<BSDF> &bsdf,
			const Spectrum &beta)
			: p(p), wo(wo), bsdf(bsdf), beta(beta) {}
		Point3f p;
		Vector3f wo;
		std::shared_ptr<BSDF> bsdf;
		Spectrum beta;
	} vp;
	AtomicFloat Phi[3];
	std::atomic<int> M;
	Float N = 0;
	Spectrum tau;
};

struct SPPMPixelListNode {
	SPPMPixel *pixel;
	SPPMPixelListNode *next;
};

static bool ToGrid(const Point3f &p, const Bounds3f &bounds,
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
		(p.z * 83492791)) %
		hashSize;
}

void SPPM::render(const Scene & scene)
{
	// init data
	auto film = camera->getFilm();
	int filmWidth = film->getWidth(), filmHeight = film->getHeight();
	int nPixels = filmWidth * filmHeight;
	std::unique_ptr<SPPMPixel[]> pixels(new SPPMPixel[nPixels]);
	for (int i = 0; i < nPixels; ++i)
		pixels[i].radius = initialSearchRadius;
	Float invSqrtSPP = 1.0f / std::sqrt(nIterations);

	ProcessReporter reporter((nPixels * 3 + photonPerIteration) * nIterations);

	// compute light distribution
	int n = (int)scene.lights.size();
	CHECK_INFO(n > 0, "no light, you will get a black picture!");
	std::vector<Float> pdfs(n);
	for (int i = 0; i < n; ++i)
		pdfs[i] = scene.lights[i]->power().intensity();
	auto lightDistrib = std::shared_ptr<Distribution1D>(new Distribution1D(pdfs.data(), n));

	for (int iter = 0; iter < nIterations; ++iter) {

		// Accumulating visible points
		{
#pragma omp parallel for
			for (int j = 0; j < filmHeight; ++j) {
				for (int i = 0; i < filmWidth; ++i) {
					auto s = sampler->clone(iter * nPixels + filmWidth * j + i);

					Ray ray = camera->generateRay(Point2f((Float)i, (Float)j), *s);
					Spectrum beta(1.0f);

					SPPMPixel &pixel = pixels[filmWidth * j + i];
					bool specularBounce = false;
					for (int depth = 0; depth < maxDepth; ++depth) {
						Intersection isec;
						if (!scene.intersect(ray, &isec)) {
							// env map
							for (const auto &light : scene.envmaps)
								pixel.Ld += beta * light->Le(ray);
							break;
						}
						isec.calculateBSDF();
						auto sdf = isec.bsdf;
						const BSDF &bsdf = *isec.bsdf;

						Vector3f wo = -ray.d;
						if (depth == 0 || specularBounce)
							pixel.Ld += beta * isec.Le(wo);
						pixel.Ld += beta * uniformSampleOneLight(ray, isec, scene, *s);

						bool isDiffuse = bsdf.numBxDF(BxDFType(
							BxDF_DIFFUSE | BxDF_REFLECTION |
							BxDF_TRANSMISSION)) > 0;
						bool isGlossy = bsdf.numBxDF(BxDFType(
							BxDF_GLOSSY | BxDF_REFLECTION |
							BxDF_TRANSMISSION)) > 0;
						if (isDiffuse || (isGlossy && depth == maxDepth - 1)) {
							pixel.vp = { isec.p, wo, isec.bsdf, beta };
							break;
						}

						if (depth < maxDepth - 1) {
							Float pdf;
							Vector3f wi;
							BxDFType type;
							Spectrum f =
								bsdf.sample_f(&wi, wo, s->next2(),
									&pdf, BxDF_ALL, &type);
							if (pdf == 0. || f.isBlack()) break;
							specularBounce = (type & BxDF_SPECULAR) != 0;
							beta *= f * absDot(wi, isec.ns) / pdf;
							if (beta.intensity() < 0.25) {
								Float continueProb = std::min((Float)1, beta.intensity());
								if (s->next() > continueProb) break;
								beta /= continueProb;
							}
							ray = isec.spawnRay(wi);
						}
					}
					reporter.done();
				}
			}
		}// Accumulating visible points

		// Visible point grid construction
		int gridRes[3];
		Bounds3f gridBounds;
		const int hashSize = nPixels;
		std::vector<std::atomic<SPPMPixelListNode *>> grid(hashSize);
		{
			Float maxRadius = 0.;
			for (int i = 0; i < nPixels; ++i) {
				const SPPMPixel &pixel = pixels[i];
				if (pixel.vp.beta.isBlack()) continue;
				Bounds3f vpBound = expand(Bounds3f(pixel.vp.p), pixel.radius);
				gridBounds = Union(gridBounds, vpBound);
				maxRadius = std::max(maxRadius, pixel.radius);
			}

			Vector3f diag = gridBounds.diagonal();
			Float maxDiag = maxComponent(diag);
			int baseGridRes = (int)(maxDiag / maxRadius);
			for (int i = 0; i < 3; ++i)
				gridRes[i] = std::max((int)(baseGridRes * diag[i] / maxDiag), 1);

#pragma omp parallel for
			for (int pixelIndex = 0; pixelIndex < nPixels; ++pixelIndex) {
				SPPMPixel &pixel = pixels[pixelIndex];
				if (!pixel.vp.beta.isBlack()) {
					Float radius = pixel.radius;
					Point3i pMin, pMax;
					ToGrid(pixel.vp.p - Vector3f(radius, radius, radius),
						gridBounds, gridRes, &pMin);
					ToGrid(pixel.vp.p + Vector3f(radius, radius, radius),
						gridBounds, gridRes, &pMax);

					int gridSize = (pMax.z - pMin.z + 1) * (pMax.y - pMin.y + 1) * (pMax.x - pMin.x + 1);
					for (int z = pMin.z; z <= pMax.z; ++z)
						for (int y = pMin.y; y <= pMax.y; ++y)
							for (int x = pMin.x; x <= pMax.x; ++x) {
								// Add visible point to grid cell $(x, y, z)$
								int h = hash(Point3i(x, y, z), hashSize);

								// remember to delete!!!
								SPPMPixelListNode *node = new SPPMPixelListNode();
								node->pixel = &pixel;

								// Atomically add _node_ to the start of
								// _grid[h]_'s linked list
								node->next = grid[h];
								while (grid[h].compare_exchange_weak(
									node->next, node) == false)
									;
							}
				}
				reporter.done();
			}
		}

		// Accumulateing photon contributions
		{
#pragma omp parallel for
			for (int photonIndex = 0; photonIndex < photonPerIteration; ++photonIndex) {
				auto s = sampler->clone(nIterations * nPixels + iter * photonPerIteration + photonIndex);

				Float lightPdf;
				int lightNum = lightDistrib->sampleDiscrete(s->next(), &lightPdf);
				const auto &light = scene.lights[lightNum];

				Point2f rand0 = s->next2(), rand1 = s->next2();
				Ray photonRay;
				Normal3f nLight;
				Float pdfPos, pdfDir;
				Spectrum Le = light->sample_Le(rand0, rand1, &photonRay,
						&nLight, &pdfPos, &pdfDir);

				if (pdfPos == 0 || pdfDir == 0 || Le.isBlack())
					continue;
				Spectrum beta = (absDot(nLight, photonRay.d) * Le) /
					(lightPdf * pdfPos * pdfDir);
				if (beta.isBlack())
					continue;

				// Follow photon path through scene and record intersections
				Intersection isec;
				for (int depth = 0; depth < maxDepth; ++depth) {
					if (!scene.intersect(photonRay, &isec))
						break;
					if (depth > 0) {
						// Add photon contribution to nearby visible points
						Point3i photonGridIndex;
						if (ToGrid(isec.p, gridBounds, gridRes, &photonGridIndex)) {
							int h = hash(photonGridIndex, hashSize);
							// Add photon contribution to visible points in
							// _grid[h]_
							for (SPPMPixelListNode *node = grid[h].load(std::memory_order_relaxed);
								node != nullptr; node = node->next) {
								SPPMPixel &pixel = *node->pixel;
								Float radius = pixel.radius;
								if ((pixel.vp.p - isec.p).lengthSquared() > radius * radius)
									continue;
								// Update _pixel_ $\Phi$ and $M$ for nearby
								// photon
								Vector3f wi = -photonRay.d;
								Spectrum Phi = beta * pixel.vp.bsdf->f(wi, pixel.vp.wo);
								pixel.Phi[0].add(Phi.r);
								pixel.Phi[1].add(Phi.g);
								pixel.Phi[2].add(Phi.b);

								++pixel.M;
							}
						}
					}

					isec.calculateBSDF(TransportMode::Photon);

					const BSDF &photonBSDF = *isec.bsdf;

					Vector3f wi, wo = -photonRay.d;
					Float pdf;
					BxDFType flags;

					Spectrum fr = photonBSDF.sample_f(&wi, wo, s->next2(), &pdf, BxDF_ALL, &flags);
					if (fr.isBlack() || pdf == 0)
						break;
					Spectrum bnew = beta * fr * absDot(wi, isec.ns) / pdf;

					Float q = std::max((Float)0, 1 - bnew.intensity() / beta.intensity());
					if (s->next() < q) break;
					beta = bnew / (1 - q);
					photonRay = isec.spawnRay(wi);
				}
				reporter.done();
			}
			for (int i = 0; i < nPixels; ++i) {
				SPPMPixelListNode *node = grid[i].load(std::memory_order_relaxed);
				while (node != nullptr) {
					SPPMPixelListNode *next = node->next;
					delete node;
					node = next;
				}
			}
		} // Accumulateing photon contributions

		// Update pixel values
		{
#pragma omp parallel for
			for (int i = 0; i < nPixels; ++i) {
				SPPMPixel &p = pixels[i];
				if (p.M > 0) {
					// Update pixel photon count, search radius, and $\tau$ from
					// photons
					Float gamma = (Float)2 / (Float)3;
					Float Nnew = p.N + gamma * p.M;
					Float Rnew = p.radius * std::sqrt(Nnew / (p.N + p.M));
					Spectrum Phi;
					Phi.r = p.Phi[0];
					Phi.g = p.Phi[1];
					Phi.b = p.Phi[2];
					p.tau = (p.tau + p.vp.beta * Phi) * (Rnew * Rnew) /
						(p.radius * p.radius);
					p.N = Nnew;
					p.radius = Rnew;
					p.M = 0;
					for (int j = 0; j < 3; ++j)
						p.Phi[j] = (Float)0;
				}
				p.vp.beta = 0.;
				p.vp.bsdf = nullptr;
				reporter.done();
			}
		} // Update pixel values

		uint64_t Np = (uint64_t)nIterations * (uint64_t)photonPerIteration;
		for (int j = 0; j < filmHeight; ++j)
			for (int i = 0; i < filmWidth; ++i) {
				const SPPMPixel &pixel = pixels[j * filmWidth + i];
				Spectrum L = pixel.Ld / nIterations;
				L += pixel.tau / (Np * pi * pixel.radius * pixel.radius);
				film->acumulate(i, j, L);
			}
	}
}

std::shared_ptr<SPPM> createSPPMIntegrator(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Sampler>& sampler, const ParamSet & param)
{
	int nIterations = parseInt(param.getParam("numIterations"));
	int maxDepth = parseInt(param.getParam("maxDepth"));
	int photonsPerIter = parseInt(param.getParam("photonsPerIteration"));
	if (photonsPerIter < 0)
		photonsPerIter = camera->getFilm()->getHeight() * camera->getFilm()->getWidth();
	Float radius = parseFloat(param.getParam("initRadius"));
	return std::make_shared<SPPM>(camera, sampler, maxDepth, nIterations, photonsPerIter, radius);
}

ORION_NAMESPACE_END
