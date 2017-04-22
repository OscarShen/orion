/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_FLOAT_TEXTURE_H_
#define ORION_FLOAT_TEXTURE_H_
#include <orion.h>
#include "texture.h"
namespace orion {

	class FloatTexture : public Texture
	{
	private:
		std::unique_ptr<Float[]> data;

	public:
		FloatTexture() {}
		FloatTexture(int width, int height) { setSize(width, height); }
		FloatTexture(int width, int height, Float v);
		FloatTexture(Float v);
		void setFloat(int x, int y, Float v);
		// To satisfy the interface, convert Float t to Spectrum(t,t,t)
		virtual Spectrum sample(int x, int y) const override;
		// Redefine the size, which will delete the original data
		virtual void setSize(int width, int height) override;
	};

}

#endif // !ORION_FLOAT_TEXTURE_H_
