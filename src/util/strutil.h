/************************************************************************ 
 * @description :  some utilities of string
 * @author		:  Oscar Shen
 * @creat 		:  2017Äê4ÔÂ28ÈÕ
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_STRING_H_
#define ORION_UTIL_STRING_H_
#include <orion.h>
#include <core/transform.h>
#include <core/spectrum.h>
#include <shape/triangle.h>
namespace orion {

	void trim(std::string &s);

	std::vector<std::string> split(const std::string &s, const std::string &delim);

	int parseInt(const std::string &s);

	Float parseFloat(const std::string &s);

	Vector2i parseVector2i(const std::string &s);

	Vector3f parseVector3f(const std::string &s);

	Vector3f parseRotate(const std::string & s, Float &degree);

	Point3f parsePoint3f(const std::string &s);

	Spectrum parseSpectrum(const std::string &s);

	Transform parseLookAt(const std::string &s);

	std::shared_ptr<MeshData> parseMeshData(const std::string &p, const std::string &uv, const std::string &n, const std::string &indices);
}

#endif // !ORION_UTIL_STRING_H_
