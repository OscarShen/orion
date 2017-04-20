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
		RGBSpectrum(Float r, Float g, Float b) :r(r), g(g), b(b) {}
		RGBSpectrum(unsigned char r, unsigned char g, unsigned char b) {
			Float inv = 1.0f / 255.0f;
			this->r = r * inv;
			this->g = g * inv;
			this->b = b * inv;
		}
		~RGBSpectrum() {}
	};

	inline std::ostream &operator<<(std::ostream &os, const RGBSpectrum &s) {
		os << "RGBSpectrum : [" << s.r << ", " << s.g << ", " << s.b << "] ";
		return os;
	}
}

#endif // !ORION_SPECTRUM_H_
