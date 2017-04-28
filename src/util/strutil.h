/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_STRING_H_
#define ORION_UTIL_STRING_H_
#include <orion.h>
#include <core/transform.h>
namespace orion {

	void trim(std::string &s);

	std::vector<std::string> split(const std::string &s, const std::string &delim);

	Vector3f parseVector3f(const std::string &s);

	Transform parseTransform(const std::string &s);

	std::string nextToken(std::string &str, char t);
}

#endif // !ORION_UTIL_STRING_H_
