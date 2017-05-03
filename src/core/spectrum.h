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
namespace orion {

	class RGBSpectrum
	{
	public:
		Float r, g, b;

	public:
		RGBSpectrum() { r = g = b = 0.0f; }
		RGBSpectrum(Float v) { r = g = b = v; }
		RGBSpectrum(Float r, Float g, Float b) :r(r), g(g), b(b) {}
		RGBSpectrum(unsigned char r, unsigned char g, unsigned char b) {
			Float inv = 1.0f / 255.0f;
			this->r = r * inv;
			this->g = g * inv;
			this->b = b * inv;
		}
		~RGBSpectrum() {}

		RGBSpectrum operator+(Float f) const {
			return RGBSpectrum(r + f, g + f, b + f);
		}
		RGBSpectrum operator-(Float f) const {
			return RGBSpectrum(r - f, g - f, b - f);
		}
		RGBSpectrum operator*(Float f) const {
			return RGBSpectrum(r * f, g * f, b * f);
		}
		RGBSpectrum operator+(const RGBSpectrum &rhs) const {
			return RGBSpectrum(rhs.r + r, rhs.g + g, rhs.b + b);
		}
		RGBSpectrum operator*(const RGBSpectrum &rhs) const {
			return RGBSpectrum(rhs.r * r, rhs.g * g, rhs.b * b);
		}
		RGBSpectrum &operator+=(const RGBSpectrum &rhs) {
			r += rhs.r;
			g += rhs.g;
			b += rhs.b;
			return *this;
		}
		RGBSpectrum &operator-=(const RGBSpectrum &rhs) {
			r -= rhs.r;
			g -= rhs.g;
			b -= rhs.b;
			return *this;
		}
	};

	inline RGBSpectrum operator*(Float f, const RGBSpectrum &s) {
		return s * f;
	}

	inline std::ostream &operator<<(std::ostream &os, const RGBSpectrum &s) {
		os << "RGBSpectrum : [" << s.r << ", " << s.g << ", " << s.b << "] ";
		return os;
	}
}

#endif // !ORION_SPECTRUM_H_
