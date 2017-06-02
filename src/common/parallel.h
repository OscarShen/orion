/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_COMMON_PARALLEL_H_
#define ORION_COMMON_PARALLEL_H_
#include <orion.h>
namespace orion {

	bool multiThreadEnabled();

	void setMultiThread(bool enable);

	int numSystemCores();

	int threadID();
}

#endif // !ORION_COMMON_PARALLEL_H_
