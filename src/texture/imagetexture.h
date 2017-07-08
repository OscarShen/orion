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
#include <core/texture.h>
namespace orion {

	struct ImageMemory
	{
		ImageMemory() {}
		ImageMemory(Spectrum *data, int width, int height)
			:data(data), width(width), height(height) {}
		~ImageMemory() { if(data) delete[] data; }
		Spectrum *data;
		int width;
		int height;
	};

	std::shared_ptr<ImageMemory> loadImage(const std::string &filename);

	// TODO : Add mipmap
	class ImageTexture : public Texture
	{
	private:
		std::shared_ptr<ImageMemory> memory;

	public:
		ImageTexture() {}
		ImageTexture(std::shared_ptr<ImageMemory> mem)
			: memory(mem)
		{
			width = mem->width;
			height = mem->height;
		}

		void setSize(int width, int height) override { /* You can't reset size of image texure */ }

		using Texture::sample;
		Spectrum sample(int x, int y) const override;

		friend class TexManager;
	};
}

#endif // !ORION_IMAGE_TEXUTRE_H_
