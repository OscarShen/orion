/************************************************************************ 
 * @description :  used for getting SPD values of material
 * @author		:  Oscar Shen
 * @creat 		:  2017-7-10 21:47:19
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_FLOAT_FILE_H_
#define ORION_CORE_FLOAT_FILE_H_

#include <orion.h>
ORION_NAMESPACE_BEGIN

bool readFloatFile(const char *filename, std::vector<Float> *values);

ORION_NAMESPACE_END


#endif // !ORION_CORE_FLOAT_FILE_H_
