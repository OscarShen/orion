/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MIX_TEXTURE_H_
#define ORION_MIX_TEXTURE_H_
#include <orion.h>
#include "texture.h"
#include "floattexture.h"
namespace orion {

	class MixTexture : public Texture
	{
	private:
		std::shared_ptr<Texture> tex1, tex2;
		std::shared_ptr<FloatTexture> amount;	// Used to store mix values, with RGB color to store the coefficient may be a bit wasteful

	public:
		MixTexture(const std::shared_ptr<Texture> &tex1,
				   const std::shared_ptr<Texture> &tex2,
				   const std::shared_ptr<FloatTexture> &amount)
			:tex1(tex1), tex2(tex2), amount(amount) {
			int width = std::max(tex1->getWidth(), tex2->getWidth()), height = std::max(tex1->getHeight(), tex2->getHeight());
			setSize(width, height);
		}
		virtual Spectrum sample(int x, int y) const override {
			auto t1 = tex1->sample(x, y), t2 = tex2->sample(x, y);
			auto amt = amount->sample(x, y);
			return t1 * (1 - amt.r) + t2 * amt.r;
		}
	};

}

#endif // !ORION_MIX_TEXTURE_H_
