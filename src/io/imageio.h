/************************************************************************ 
 * @description :  load & write image
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-20 22:42:04
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_IMAGE_IO_H_
#define ORION_IMAGE_IO_H_
#include <orion.h>
#include <texture/imagetexture.h>
#include <stb_image/stb_image_aug.h>
namespace orion {

	bool loadImage(const std::string &filename, std::shared_ptr<ImageMemory> &mem);
	unsigned char * loadImage(const std::string & filename, int  &width, int  &height, int &channels);
	void freeImage(unsigned char *data);

	bool saveAsBmp(const Texture * texture, const std::string & str);
}

#endif // !ORION_IMAGE_IO_H_
