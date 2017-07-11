/************************************************************************
* @description :  rgb spectrum
* @author		:  Oscar Shen
* @creat 		:  2017-4-18 21:51:38
************************************************************************
* Copyright @ OscarShen 2017. All rights reserved.
************************************************************************/
#pragma once
#ifndef ORION_SPECTRUM_H_
#define ORION_SPECTRUM_H_
#include <orion.h>
#include "geometry.h"

ORION_NAMESPACE_BEGIN

// all these functions are from PBRT
static const int sampledLambdaStart = 400;
static const int sampledLambdaEnd = 700;
static const int nSpectralSamples = 60;
extern bool SpectrumSamplesSorted(const Float *lambda, const Float *vals,
	int n);
extern void SortSpectrumSamples(Float *lambda, Float *vals, int n);
extern Float AverageSpectrumSamples(const Float *lambda, const Float *vals,
	int n, Float lambdaStart, Float lambdaEnd);
extern Float InterpolateSpectrumSamples(const Float *lambda, const Float *vals,
	int n, Float l);
static const int nCIESamples = 471;
extern const Float CIE_X[nCIESamples];
extern const Float CIE_Y[nCIESamples];
extern const Float CIE_Z[nCIESamples];
extern const Float CIE_lambda[nCIESamples];
static const Float CIE_Y_integral = 106.856895;

inline void XYZToRGB(const Float xyz[3], Float rgb[3]) {
	rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
	rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
	rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
}

inline void RGBToXYZ(const Float rgb[3], Float xyz[3]) {
	xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
	xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
	xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
}
// - all these functions are from PBRT

class Spectrum
{
public:
	Float r, g, b;

public:
	Spectrum() { r = g = b = 0.0f; }
	Spectrum(Float v) { r = g = b = v; }
	Spectrum(Float r, Float g, Float b) :r(r), g(g), b(b) {}
	Spectrum(unsigned char r, unsigned char g, unsigned char b) {
		Float inv = 1.0f / 255.0f;
		this->r = r * inv;
		this->g = g * inv;
		this->b = b * inv;
	}
	~Spectrum() {}
	bool isBlack() const {
		if (r != 0.0f || g != 0.0f || b != 0.0f)
			return false;
		return true;
	}

	Float maxComponentValue() const {
		return std::max(r, std::max(g, b));
	}

	Float intensity() const {
		const Float yWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
		return yWeight[0] * r + yWeight[1] * g + yWeight[2] * b;
	}

	Spectrum clamp(Float low = 0, Float high = fInfinity) const {
		return Spectrum(orion::clamp(r, low, fInfinity),
			orion::clamp(g, low, fInfinity),
			orion::clamp(b, low, fInfinity));
	}

	static Spectrum fromXYZ(const Float xyz[3]) {
		Spectrum r;
		XYZToRGB(xyz, (Float*)&r);
		return r;
	}

	static Spectrum fromSampled(const Float *lambda, const Float *v, int n) {
		// Sort samples if unordered, use sorted for returned spectrum
		if (!SpectrumSamplesSorted(lambda, v, n)) {
			std::vector<Float> slambda(&lambda[0], &lambda[n]);
			std::vector<Float> sv(&v[0], &v[n]);
			SortSpectrumSamples(&slambda[0], &sv[0], n);
			return fromSampled(&slambda[0], &sv[0], n);
		}
		Float xyz[3] = { 0, 0, 0 };
		for (int i = 0; i < nCIESamples; ++i) {
			Float val = InterpolateSpectrumSamples(lambda, v, n, CIE_lambda[i]);
			xyz[0] += val * CIE_X[i];
			xyz[1] += val * CIE_Y[i];
			xyz[2] += val * CIE_Z[i];
		}
		Float scale = Float(CIE_lambda[nCIESamples - 1] - CIE_lambda[0]) /
			Float(CIE_Y_integral * nCIESamples);
		xyz[0] *= scale;
		xyz[1] *= scale;
		xyz[2] *= scale;
		return fromXYZ(xyz);
	}

	template <typename T>
	Spectrum operator+(T f) const {
		return Spectrum(r + f, g + f, b + f);
	}
	template <typename T>
	Spectrum operator-(T f) const {
		return Spectrum(r - f, g - f, b - f);
	}
	template <typename T>
	Spectrum operator*(T f) const {
		return Spectrum(r * f, g * f, b * f);
	}
	template <typename T>
	Spectrum operator/(T f) const {
		CHECK_NE(f, 0);
		return Spectrum(r / f, g / f, b / f);
	}
	template <typename T>
	Spectrum &operator/=(T f) {
		CHECK_NE(f, 0);
		r /= f;
		g /= f;
		b /= f;
		return *this;
	}
	Spectrum operator-(const Spectrum &rhs) const {
		return Spectrum(r - rhs.r, g - rhs.g, b - rhs.b);
	}
	Spectrum operator+(const Spectrum &rhs) const {
		return Spectrum(rhs.r + r, rhs.g + g, rhs.b + b);
	}
	Spectrum operator*(const Spectrum &rhs) const {
		return Spectrum(rhs.r * r, rhs.g * g, rhs.b * b);
	}
	Spectrum operator/(const Spectrum &rhs) const {
		CHECK_INFO(!rhs.isBlack(), "spectrum divide by zero!");
		return Spectrum(r / rhs.r, g / rhs.g, b / rhs.b);
	}
	Spectrum &operator*=(const Spectrum &rhs) {
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		return *this;
	}
	Spectrum &operator+=(const Spectrum &rhs) {
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		return *this;
	}
	Spectrum &operator-=(const Spectrum &rhs) {
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		return *this;
	}
	Spectrum operator-() const {
		Spectrum ret;
		ret.r = -r;
		ret.g = -g;
		ret.b = -b;
		return ret;
	}

	friend Spectrum sqrt(const Spectrum &s) {
		return Spectrum(
			std::sqrt(s.r),
			std::sqrt(s.g),
			std::sqrt(s.b));
	}
};

template <typename T>
inline Spectrum operator*(T f, const Spectrum &s) {
	return s * f;
}

inline std::ostream &operator<<(std::ostream &os, const Spectrum &s) {
	os << "Spectrum : [" << s.r << ", " << s.g << ", " << s.b << "] ";
	return os;
}


ORION_NAMESPACE_END
#endif // !ORION_SPECTRUM_H_
