/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UV_TEXTURE_H_
#define ORION_UV_TEXTURE_H_
#include <orion.h>
#include "texture.h"
namespace orion {

	class UVTexture : public Texture
	{
	public:
		UVTexture() { Texture::setSize(16, 16); }
		virtual ~UVTexture() {}

		virtual Spectrum sample(int x, int y) const override;

	};

}

#endif // !ORION_UV_TEXTURE_H_
