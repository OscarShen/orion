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
		Float padding;

	public:
		GridTexture(const Spectrum &s0, const Spectrum &s1, Float padding = 0.1f)
			:s0(s0), s1(s1), padding(padding) {
			Texture::setSize(16, 16);
		}
		GridTexture(int width, int height, const Spectrum &s0, const Spectrum &s1, Float padding = 0.1f)
			: s0(s0), s1(s1), padding(padding)
		{
			Texture::setSize(width, height);
		}

		virtual Spectrum sample(int x, int y) const override {
			_coordFilter(x, y);
			//int half_width = width / 2, half_height = height / 2;

			int delta_x = (x - width / 2);
			int delta_y = (y - height / 2);

			// the size for the center grid
			float w_size = (width * 0.5f * (1 - padding));
			float h_size = (height * 0.5f * (1 - padding));

			if (delta_x < w_size && delta_x >= -w_size &&
				delta_y < h_size && delta_y >= -h_size)

			//if ((x < half_width * 0.9f && y < half_height * 0.9f) || (
			//	x >= half_width * 1.1f && y >= half_height * 1.1f))
				return s0;
			else
				return s1;
		}
	};

}


#endif // !ORION_GRID_TEXTURE_H_
