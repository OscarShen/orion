/************************************************************************ 
 * @description :  metal material
 * @author		:  Oscar Shen
 * @creat 		:  2017-7-11 16:33:42
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_METAL_H_
#define ORION_MATERIAL_METAL_H_

#include <core/material.h>
ORION_NAMESPACE_BEGIN

class Metal : public Material
{
private:
	std::shared_ptr<Texture> eta, k;
	std::shared_ptr<FloatTexture> roughness;

public:
	Metal(const std::shared_ptr<Texture> &eta,
		const std::shared_ptr<Texture> &k,
		const std::shared_ptr<FloatTexture> &roughness)
		:eta(eta), k(k), roughness(roughness) {}
	std::shared_ptr<BSDF> getBSDF(const Intersection &isec, TransportMode mode = TransportMode::Path) const override;
};

std::shared_ptr<Metal> createMetalMaterial(const std::shared_ptr<Texture> &eta, const std::shared_ptr<Texture> &k,
	const std::shared_ptr<FloatTexture> &roughness, const ParamSet &param);

ORION_NAMESPACE_END

#endif // !ORION_MATERIAL_METAL_H_
