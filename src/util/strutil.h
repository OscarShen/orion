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

	Float parseFloat(const std::string &s);

	Vector2i parseVector2i(const std::string &s);

	Vector3f parseVector3f(const std::string &s);

	Point3f parsePoint3f(const std::string &s);
}

#endif // !ORION_UTIL_STRING_H_
