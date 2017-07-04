/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_GLASS_H_
#define ORION_MATERIAL_GLASS_H_
#include <orion.h>
#include "material.h"
#include <texture/texture.h>
namespace orion {

	class GlassMaterial : public Material
	{
	private:
		std::shared_ptr<Texture> Kr, Kt;		// image or constant
		std::shared_ptr<FloatTexture> roughness;	// image or constant
		std::shared_ptr<FloatTexture> eta;			// float texture

	public:
		GlassMaterial(const std::shared_ptr<Texture> &Kr,
					  const std::shared_ptr<Texture> &Kt,
					  const std::shared_ptr<FloatTexture> roughness,
					  const std::shared_ptr<FloatTexture> eta) : eta(eta), Kr(Kr), Kt(Kt), roughness(roughness) {}
		std::shared_ptr<BSDF> getBSDF(const Intersection *isec, bool hasEtaAtenuation = true) const override;
	};

	std::shared_ptr<GlassMaterial> createGlassMaterial(const std::shared_ptr<Texture> &Kr,
		const std::shared_ptr<Texture> &Kt,
		const std::shared_ptr<FloatTexture> roughness,
		const std::shared_ptr<FloatTexture> eta);
}


#endif // !ORION_MATERIAL_GLASS_H_
