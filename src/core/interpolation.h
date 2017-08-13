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
Float sampleCatmullRom2D(int size1, int size2, const Float *nodes1,
	const Float *nodes2, const Float *values,
	const Float *cdf, Float alpha, Float u, Float *fval = nullptr,
	Float *pdf = nullptr);
Float integrateCatmullRom(int n, const Float *nodes, const Float *values, Float *cdf);
Float invertCatmullRom(int n, const Float *x, const Float *values, Float u);
ORION_NAMESPACE_END
#endif // !oRION_CORE_INTERPOLATION_H_
