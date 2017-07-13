/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_UBER_H_
#define ORION_MATERIAL_UBER_H_

#include <core/material.h>
ORION_NAMESPACE_BEGIN

class Uber : public Material
{
private:
	std::shared_ptr<Texture> Kd, Ks, Kr, Kt, opacity;
	std::shared_ptr<FloatTexture> roughness, eta;

public:
	Uber(const std::shared_ptr<Texture> &Kd,
		 const std::shared_ptr<Texture> &Ks,
		 const std::shared_ptr<Texture> &Kr,
		 const std::shared_ptr<Texture> &Kt,
		 const std::shared_ptr<Texture> &opacity,
		 const std::shared_ptr<FloatTexture> &roughness,
		 const std::shared_ptr<FloatTexture> &eta)
		: Kd(Kd), Ks(Ks), Kr(Kr), Kt(Kt), opacity(opacity), roughness(roughness), eta(eta) {}

	std::shared_ptr<BSDF> getBSDF(const Intersection &isec, TransportMode mode = TransportMode::Path) const override;
};

std::shared_ptr<Uber> createUberMaterial(const std::shared_ptr<Texture> &Kd,
										 const std::shared_ptr<Texture> &Ks,
										 const std::shared_ptr<Texture> &Kr,
										 const std::shared_ptr<Texture> &Kt,
										 const std::shared_ptr<Texture> &opacity,
										 const std::shared_ptr<FloatTexture> &roughness,
										 const std::shared_ptr<FloatTexture> &eta);

ORION_NAMESPACE_END

#endif // !ORION_MATERIAL_UBER_H_
