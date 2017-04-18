/************************************************************************ 
 * @description :  save texture data to hard drive
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-18 14:33:08
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORT_TEXSAVER_H_
#define ORT_TEXSAVER_H_
#include <ort.h>
namespace ort {
	// forward declaration
	class Texture;

	enum class SaveFormat { bmp };

	// output texture
	// param texture : texture to be saved
	// param str : save name
	bool saveAsBmp(const Texture *texture, const std::string &str);

}

#endif // !ORT_TEXSAVER_H_
