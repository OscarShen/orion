/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SAMPLER_SAMPLING_H_
#define ORION_SAMPLER_SAMPLING_H_
#include <orion.h>
#include <core/geometry.h>
ORION_NAMESPACE_BEGIN

inline Float cosTheta(const Vector3f &w) { return w.z; }
inline Float cos2Theta(const Vector3f &w) { return w.z * w.z; }
inline Float absCosTheta(const Vector3f &w) { return std::abs(w.z); }
inline Float sin2Theta(const Vector3f &w) { return std::max(0.0f, 1.0f - cos2Theta(w)); }
inline Float sinTheta(const Vector3f &w) { return std::sqrt(sin2Theta(w)); } // sin theta never less than zero
inline Float tanTheta(const Vector3f &w) { return sinTheta(w) / cosTheta(w); }
inline Float tan2Theta(const Vector3f &w) { return sin2Theta(w) / cos2Theta(w); }
inline Float cosPhi(const Vector3f &w) {
	Float sinTh = sinTheta(w);
	return (sinTheta == 0) ? 1.0f : clamp(w.x / sinTh, -1.0f, 1.0f);
}
inline Float sinPhi(const Vector3f &w) {
	Float sinTh = sinTheta(w);
	return (sinTheta == 0) ? 0 : clamp(w.y / sinTh, -1.0f, 1.0f);
}
inline Float cos2Phi(const Vector3f &w) { return cosPhi(w) * cosPhi(w); }
inline Float sin2Phi(const Vector3f &w) { return sinPhi(w) * sinPhi(w); }

Vector3f uniformSampleSphere(const Point2f &rand);
inline Float uniformSpherePdf() { return inv4pi; }

Point2f uniformSampleTriangle(const Point2f &rand);

Vector3f uniformSampleCone(const Point2f &rand, Float cosThetaMax);
inline Float uniformConePdf(Float cosThetaMax) {
	return 1 / (2 * pi * (1 - cosThetaMax));
}

Point2f uniformSampleDisk(const Point2f &u);
Point2f concentricSampleDisk(const Point2f &rand);
Vector3f cosineSampleHemisphere(const Point2f &rand);
inline Float cosineHemispherePdf(Float cosTheta) { return cosTheta * invpi; }

// MIS
inline Float powerHeuristic(int nf, Float fPdf, int ng, Float gPdf) {
	Float f = nf * fPdf, g = ng * gPdf;
	return (f * f) / (f * f + g * g);
}

inline bool sameHemisphere(const Vector3f &w, const Vector3f &wp) {
	return w.z * wp.z > 0;
}

inline bool sameHemisphere(const Vector3f &w, const Normal3f &wp) {
	return w.z * wp.z > 0;
}

inline Float sphericalTheta(const Vector3f &v) {
	return std::acos(clamp(v.z, -1.0f, 1.0f));
}

inline Float sphericalPhi(const Vector3f &v) {
	Float p = std::atan2(v.y, v.x);
	return (p < 0) ? (p + 2 * pi) : p;
}

struct Distribution1D
{
	std::vector<Float> func, cdf;
	Float funcInt;

	Distribution1D(const Float *f, int n) : func(f, f + n), cdf(n + 1) {
		cdf[0] = 0;
		for (int i = 1; i < n + 1; ++i)
			cdf[i] = cdf[i - 1] + func[i - 1] / n;

		funcInt = cdf[n];
		if (funcInt == 0) {
			for (int i = 1; i < n + 1; ++i)
				cdf[i] = Float(i) / Float(n);
		}
		else {
			for (int i = 1; i < n + 1; ++i)
				cdf[i] /= funcInt;
		}
	}
	int count() const { return (int)func.size(); }
	Float sampleContinuous(Float rand, Float *pdf, int *off = nullptr) const {
		// Find surrounding CDF segments and _offset_
		int offset = findInterval((int)cdf.size(),
			[&](int index) { return cdf[index] <= rand; });
		if (off) *off = offset;
		// Compute offset along CDF segment
		Float du = rand - cdf[offset];
		if ((cdf[offset + 1] - cdf[offset]) > 0) {
			du /= (cdf[offset + 1] - cdf[offset]);
		}
		CHECK_INFO(!std::isnan(du), "du should not be nan!");

		// Compute PDF for sampled offset
		if (pdf) *pdf = (funcInt > 0) ? func[offset] / funcInt : 0;

		// Return $x\in{}[0,1)$ corresponding to sample
		return (offset + du) / count();
	}
	int sampleDiscrete(Float u, Float *pdf = nullptr,
		Float *uRemapped = nullptr) const {
		// Find surrounding CDF segments and _offset_
		int offset = findInterval((int)cdf.size(),
			[&](int index) { return cdf[index] <= u; });
		if (pdf) *pdf = (funcInt > 0) ? func[offset] / (funcInt * count()) : 0;
		if (uRemapped)
			*uRemapped = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
		if (uRemapped) CHECK(*uRemapped >= 0.f && *uRemapped <= 1.f);
		return offset;
	}
	Float discretePdf(int index) const {
		CHECK(index >= 0 && index < count());
		return func[index] / (funcInt * count());
	}
};

class Distribution2D {
public:
	// Distribution2D Public Methods
	Distribution2D(const Float *data, int nu, int nv);
	Point2f SampleContinuous(const Point2f &u, Float *pdf) const {
		Float pdfs[2];
		int v;
		Float d1 = pMarginal->sampleContinuous(u[1], &pdfs[1], &v);
		Float d0 = pConditionalV[v]->sampleContinuous(u[0], &pdfs[0]);
		*pdf = pdfs[0] * pdfs[1];
		return Point2f(d0, d1);
	}
	Float Pdf(const Point2f &p) const {
		int iu = clamp(int(p[0] * pConditionalV[0]->count()), 0,
			pConditionalV[0]->count() - 1);
		int iv =
			clamp(int(p[1] * pMarginal->count()), 0, pMarginal->count() - 1);
		return pConditionalV[iv]->func[iu] / pMarginal->funcInt;
	}

private:
	// Distribution2D Private Data
	std::vector<std::unique_ptr<Distribution1D>> pConditionalV;
	std::unique_ptr<Distribution1D> pMarginal;
};

ORION_NAMESPACE_END

#endif // !ORION_SAMPLER_SAMPLING_H_
