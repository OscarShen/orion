/************************************************************************ 
 * @description :  All rendered images will be stored in _RenderTarget_
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_RENDER_TARGET_H_
#define ORION_RENDER_TARGET_H_
#include <orion.h>
#include <texture/texture.h>
namespace orion {

	class RenderTarget : public Texture
	{
	private:
		std::unique_ptr<Spectrum[]> data;

	public:
		RenderTarget() {}
		RenderTarget(int width, int height) { setSize(width, height); }
		virtual ~RenderTarget() {}
		virtual Spectrum sample(int x, int y) const override;

		// Redefine the size, which will delete the original data
		// param width,height : width & height of _RenderTarget_
		virtual void setSize(int width, int height);

		// Set the Spectrum value at the specified location
		void setSpectrum(int x, int y, const Spectrum &s);
	};

}

#endif // !ORION_RENDER_TARGET_H_
