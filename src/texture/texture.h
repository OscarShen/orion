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
#include "texsaver.h"
namespace orion {
	class Texture
	{
	public:
		// save texture to hard drive
		// param name : save name
		// param format : save as one of designate format in _SaveFormat_
		bool output(const std::string &name, SaveFormat format = SaveFormat::bmp);
	};
}

#endif // !ORT_TEXTURE_H_
