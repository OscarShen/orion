/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_NORMAL_TEXTURE_H_
#define ORION_NORMAL_TEXTURE_H_
#include <orion.h>
#include "texture.h"
namespace orion {

	class NormalTexture : public Texture
	{
	public:
		NormalTexture() {}
		~NormalTexture() {}

		virtual Spectrum sample(int x, int y) const override;
		virtual Spectrum evaluate(const Intersection *isec) const override;
	};
}

#endif // !ORION_NORMAL_TEXTURE_H_
