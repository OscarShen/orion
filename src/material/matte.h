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
#include <orion.h>
#include <common/paramset.h>
#include "material.h"
namespace orion {

	class MatteMaterial : public Material
	{
	private:
		std::shared_ptr<Texture> Kd;
		std::shared_ptr<FloatTexture> sigma;

	public:
		MatteMaterial(const std::shared_ptr<Texture> &Kd, const std::shared_ptr<FloatTexture> &sigma) :Kd(Kd), sigma(sigma) {}
		~MatteMaterial() {}
		virtual std::shared_ptr<BSDF> getBSDF(const Intersection *isec) const override;
	};

	std::shared_ptr<Material> createMatteMaterial(const ParamSet &param);
}

#endif // !ORION_MATERIAL_MATTE_H_
