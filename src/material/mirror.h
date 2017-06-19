/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_MIRROR_H_
#define ORION_MATERIAL_MIRROR_H_
#include <orion.h>
#include "material.h"
namespace orion {

	class Mirror : public Material
	{
	public:
		Mirror() {}
		~Mirror() {}

		virtual std::shared_ptr<BSDF> getBSDF(const Intersection *isec, bool hasEtaAtenuation = true) const override;
	};

	std::shared_ptr<Mirror> createMirrorMaterial();

}


#endif // !ORION_MATERIAL_MIRROR_H_
