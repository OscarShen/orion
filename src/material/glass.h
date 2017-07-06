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

#include <core/material.h>
ORION_NAMESPACE_BEGIN

class Glass : public Material
{
private:
	std::shared_ptr<Texture> Kt;
	std::shared_ptr<FloatTexture> eta;

public:
	Glass(const std::shared_ptr<Texture> &Kt, const std::shared_ptr<FloatTexture> &eta)
		: Kt(Kt), eta(eta) {}

	std::shared_ptr<BSDF> getBSDF(const Intersection &isec, TransportMode mode) const override;
};

std::shared_ptr<Glass> createGlassMaterial(const std::shared_ptr<Texture> &Kt,
										   const std::shared_ptr<FloatTexture> &eta);
ORION_NAMESPACE_END

#endif // !ORION_MATERIAL_GLASS_H_
