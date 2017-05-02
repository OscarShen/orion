/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_MERL_H_
#define ORION_MATERIAL_MERL_H_
#include <orion.h>
#include <bsdf/merl.h>
#include "material.h"
namespace orion {

	class MerlMaterial : public Material
	{
	private:
		std::shared_ptr<Merl> bxdf;

	public:
		MerlMaterial() {}
		MerlMaterial(const std::shared_ptr<Merl> &bxdf) : bxdf(bxdf) {}
		void setBxdf(const std::shared_ptr<Merl> &bxdf) { this->bxdf = bxdf; }

		virtual std::shared_ptr<BSDF> getBSDF(const Intersection *isec) const override;
	};

	std::shared_ptr<MerlMaterial> createMerlMaterial(const ParamSet &param);
}

#endif // !ORION_MATERIAL_MERL_H_
