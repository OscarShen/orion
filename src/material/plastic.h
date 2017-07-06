/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_PLASTIC_H_
#define ORION_MATERIAL_PLASTIC_H_

#include <core/material.h>
ORION_NAMESPACE_BEGIN

class Plastic : public Material
{
private:
	std::shared_ptr<Texture> Kd, Ks;
	std::shared_ptr<FloatTexture> roughness;

public:
	Plastic(const std::shared_ptr<Texture> Kd,
		std::shared_ptr<Texture> Ks,
		std::shared_ptr<FloatTexture> roughness)
		:Kd(Kd), Ks(Ks), roughness(roughness) {}

	std::shared_ptr<BSDF> getBSDF(const Intersection &isec, TransportMode mode = TransportMode::Path) const override;
};

std::shared_ptr<Plastic> createPlasticMaterial(const std::shared_ptr<Texture> &Kd,
											   const std::shared_ptr<Texture> &Ks,
											   const std::shared_ptr<FloatTexture> &roughness);
ORION_NAMESPACE_END

#endif // !ORION_MATERIAL_PLASTIC_H_
