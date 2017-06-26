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
		return RGBSpectrum(
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
