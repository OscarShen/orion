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
namespace orion {

	class GlassMaterial : public Material
	{
	private:
		Float eta;

	public:
		GlassMaterial(Float eta) : eta(eta) {}
		std::shared_ptr<BSDF> getBSDF(const Intersection *isec) const override;
	};

	std::shared_ptr<GlassMaterial> createGlassMaterial(const ParamSet &param);
}


#endif // !ORION_MATERIAL_GLASS_H_
