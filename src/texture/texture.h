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
#include <math/linalg.h>
#include <core/spectrum.h>
namespace orion {
	bool saveAsBmp(const Texture *texture, const std::string &str);

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
		Texture() { _init(); }
		virtual Spectrum sample(int x, int y) const = 0;
		virtual ~Texture() {}

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		void setFilter(TextureFilter filter) { this->filter = filter; }

		// TODO¡¡: bad impl
		virtual void setWidth(int width) { this->width = width; }
		virtual void setHeight(int height) { this->height = height; }
		virtual void setSize(int width, int height) { this->width = width; this->height = height; }

	protected:
		void _coordFilter(int &u, int &v) const;

	private:
		void _init() {
			width = 0;
			height = 0;
			filter = TextureFilter::FILTER_WRAP;
		}
	};
}

#endif // !ORT_TEXTURE_H_
