/************************************************************************ 
 * @description :  grid texture
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-18 21:50:43
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_GRID_TEXTURE_H_
#define ORION_GRID_TEXTURE_H_
#include <orion.h>
#include <core/spectrum.h>
#include "texture.h"
namespace orion {

	class GridTexture : public Texture
	{
	private:
		Spectrum s0;
		Spectrum s1;

	public:
		GridTexture(const Spectrum &s0, const Spectrum &s1)
			:s0(s0), s1(s1) {
			_init();
		}

		virtual Spectrum sample(int x, int y) const override {
			_coordFilter(x, y);
			int half_width = width / 2, half_height = height / 2;
			if ((x < half_width && y < half_height) || (
				x >= half_width && y >= half_height))
				return s0;
			else
				return s1;
		}

	private:
		void _init() {
			width = 16;
			height = 16;
		}
	};

}


#endif // !ORION_GRID_TEXTURE_H_
