/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_INTERPOLATION_H_
#define ORION_CORE_INTERPOLATION_H_
#include <orion.h>
#include "geometry.h"

ORION_NAMESPACE_BEGIN
bool catmullRomWeights(int size, const Float *nodes, Float x, int *offset,
	Float *weights);
ORION_NAMESPACE_END
#endif // !oRION_CORE_INTERPOLATION_H_
