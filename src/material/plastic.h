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
#include <orion.h>
#include "material.h"
namespace orion {

	class PlasticMaterial : public Material
	{
	private:
		std::shared_ptr<Texture> Kd, Ks;
		std::shared_ptr<Texture> roughness;

	public:
		PlasticMaterial(const std::shared_ptr<Texture> &Kd,
						const std::shared_ptr<Texture> &Ks,
						const std::shared_ptr<Texture> &roughness)
			:Kd(Kd), Ks(Ks), roughness(roughness) {}

		virtual std::shared_ptr<BSDF> getBSDF(const Intersection *isec) const override;
	};

	std::shared_ptr<PlasticMaterial> createPlasticMaterial(std::shared_ptr<Texture> &Kd,
		std::shared_ptr<Texture> &Ks,
		std::shared_ptr<Texture> &roughness);
}

#endif // !ORION_MATERIAL_PLASTIC_H_
