/************************************************************************ 
 * @description :  texture base class
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-18 14:34:08
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_TEXTURE_H_
#define ORION_TEXTURE_H_
#include <orion.h>
#include <core/spectrum.h>
#include <core/intersection.h>
namespace orion {
	enum class TextureFilter
	{
		FILTER_WRAP,
		FILTER_CLAMP,
		FILTER_MIRROR
	};

	class Texture
	{
	protected:
		int width, height;
		TextureFilter filter;

	public:
		Texture() {
			width = 0;
			height = 0;
			filter = TextureFilter::FILTER_WRAP;
		}
		virtual Spectrum sample(int x, int y) const = 0;
		virtual Spectrum evaluate(const Intersection *isec) const;

		virtual ~Texture() {}

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		void setFilter(TextureFilter filter) { this->filter = filter; }

		virtual void setSize(int width, int height) { this->width = width; this->height = height; }

	protected:
		void _coordFilter(int &u, int &v) const;
	};

	bool saveAsBmp(const Texture * texture, const std::string & str);
}

#endif // !ORT_TEXTURE_H_
