/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_H_
#define ORION_MATERIAL_H_
#include <orion.h>
#include <core/intersection.h>
#include <bsdf/bsdf.h>
namespace orion {

	class Material
	{
	public:
		Material() {}
		virtual ~Material() {}

		virtual std::shared_ptr<BSDF> getBSDF(const Intersection *isec, bool hasEtaAtenuation = true) const = 0;
	};

}

#endif // !ORION_MATERIAL_H_
