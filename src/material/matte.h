/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_MATTE_H_
#define ORION_MATERIAL_MATTE_H_

#include <core/material.h>
ORION_NAMESPACE_BEGIN

class Matte : public Material
{
private:
	std::shared_ptr<Texture> Kd;
	std::shared_ptr<FloatTexture> sigma;

public:
	Matte(std::shared_ptr<Texture> Kd, std::shared_ptr<FloatTexture> sigma)
		:Kd(Kd), sigma(sigma) {}

	void getBSDF(Intersection *isec, TransportMode mode) const override;
};

std::shared_ptr<Matte> createMatteMaterial(std::shared_ptr<Texture> Kd, std::shared_ptr<FloatTexture> sigma);

ORION_NAMESPACE_END

#endif // !ORION_MATERIAL_MATTE_H_
