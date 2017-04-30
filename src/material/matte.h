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
		std::shared_ptr<Texture> diffuse;

	public:
		MatteMaterial(const std::shared_ptr<Texture> &diffuse) :diffuse(diffuse) {}
		~MatteMaterial() {}
		virtual std::shared_ptr<BSDF> getBSDF(const Intersection *isec) const override;
		void setDiffuse(const std::shared_ptr<Texture> &diffuse) { this->diffuse = diffuse; }
	};

	std::shared_ptr<Material> createMatteMaterial(const ParamSet &param);
}

#endif // !ORION_MATERIAL_MATTE_H_
