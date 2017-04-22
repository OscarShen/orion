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

#ifdef ERROR
#undef ERROR

#ifdef ORION_IN_WINDOWS
#include <Windows.h>
#endif // ORION_IN_WINDOWS

namespace orion {

	std::string getExecutiveDir()
	{
		const int maxLen = 2048;
		char buf[maxLen];

#if defined(ORION_IN_LINUX)
		int c = readlink("/proc/self/exe", buf, maxLen - 1);
#elif defined(ORION_IN_WINDOWS)
		// get the current module filename
		GetModuleFileNameA(NULL, buf, maxLen);

		// remove the file name
		int len = (int)strlen(buf);
		for (int i = len - 1; i > 0; i--)
		{
			if (buf[i] == '\\')
			{
				buf[i + 1] = 0;
				break;
			}
		}
#endif
		return std::string(buf);
	}
#endif // ERROR
}

#endif // !ORION_ENVIRONMENT_VARIBLE_H_
