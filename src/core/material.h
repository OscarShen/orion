/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_MATERIAL_H_
#define ORION_CORE_MATERIAL_H_

#include <orion.h>
ORION_NAMESPACE_BEGIN

enum class TransportMode { Path, Photon };

class Material
{
public:
	virtual ~Material() {}
	virtual std::shared_ptr<BSDF> getBSDF(const Intersection &isec, TransportMode mode = TransportMode::Path) const = 0;
};

ORION_NAMESPACE_END

#endif // !ORION_CORE_MATERIAL_H_
