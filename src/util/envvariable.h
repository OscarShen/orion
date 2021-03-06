/************************************************************************ 
 * @description :  environment variables
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-22 13:59:14
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_ENVIRONMENT_VARIBLE_H_
#define ORION_ENVIRONMENT_VARIBLE_H_
#include <orion.h>

#ifdef ORION_IN_WINDOWS
#define NOMINMAX
#include <Windows.h>
#endif // ORION_IN_WINDOWS

namespace orion {

	std::string getExecutiveDir();

	std::string getFullPath(const std::string &name);

	void setResPath(const std::string &path);

	std::string getResPath();
}
#endif // !ORION_ENVIRONMENT_VARIBLE_H_
