/************************************************************************ 
 * @description :  texture has constant spectrum
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-18 19:06:53
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CONSTANT_TEXTURE_H_
#define ORION_CONSTANT_TEXTURE_H_
#include <orion.h>
#include <core/spectrum.h>
#include "texture.h"
namespace orion {

	class ConstantTexture : public Texture
	{
	private:
		Spectrum s;

	public:
		ConstantTexture() {}
		ConstantTexture(Float r, Float g, Float b) : s(r,g,b) {}
		ConstantTexture(const Spectrum &s) : s(s) {}
		~ConstantTexture() {}
		virtual Spectrum sample(int x, int y) const override {
			return s;
		}
		
	private:
		void _init() {
			width = 16;
			height = 16;
		}
	};

}

#endif // !ORION_CONSTANT_TEXTURE_H_
