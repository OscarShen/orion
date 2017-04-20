/************************************************************************ 
 * @description :  image texture
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-20 22:41:32
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_IMAGE_TEXUTRE_H_
#define ORION_IMAGE_TEXUTRE_H_
#include <orion.h>
#include <texture/texture.h>
namespace orion {

	struct ImageMemory
	{
		ImageMemory() {}
		ImageMemory(Spectrum *data, int width, int height)
			:data(data), width(width), height(height) {}
		~ImageMemory() { delete[] data; }
		Spectrum *data;
		int width;
		int height;
	};

	class ImageTexture : public Texture
	{
	private:
		std::shared_ptr<ImageMemory> memory;

	public:
		virtual void setWidth(int width) override {}
		virtual void setHeight(int width) override {}
		virtual void setSize(int width, int height) override {}

		virtual Spectrum sample(int x, int y) const override;

		virtual ~ImageTexture() {}

	private:
		ImageTexture() {}
		ImageTexture(std::shared_ptr<ImageMemory> mem)
			: memory(mem)
		{
			width = mem->width;
			height = mem->height;
		}

		friend class TexManager;
	};


}

#endif // !ORION_IMAGE_TEXUTRE_H_
