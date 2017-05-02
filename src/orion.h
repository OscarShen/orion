/************************************************************************ 
 * @description :  全局设置头文件
 * @author		:  Oscar Shen
 * @creat 		:  2017年4月18日09:02:56
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_H_
#define ORION_H_
#include <algorithm>
#include <memory>
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <sstream>
#include <fstream>
namespace orion {

#if defined(_WIN32) || defined(_WIN64)
#define ORION_IN_WINDOWS
#elif defined(__linux__)
#define ORION_IN_LINUX
#endif

#define CHECK_INFO(x, str) if(!(x)) { std::cout << (str) << "\n\t|file: "<< __FILE__<<",line: " << __LINE__<<std::endl;}
#define CHECK(x) CHECK_INFO(x, "check equal failed!")
#define CHECK_EQ(x, y) if((x)!=(y)) { std::cout << "check equal failed!" << "\n\t|file: "<< __FILE__<<",line: " << __LINE__<<std::endl; }
#define CHECK_NE(x, y) if((x)==(y)) { std::cout << "check not equal failed!" << "\n\t|file: "<< __FILE__<<",line: " << __LINE__<<std::endl; }
#define ERROR(info) CHECK_INFO(false, info)

	typedef float Float;

	constexpr Float epsilon = 1e-6f;

	constexpr Float fInfinity = std::numeric_limits<Float>::max();
	constexpr Float pi = 3.14159265358979323846f;
	constexpr Float invpi = 0.31830988618379067154f;
	constexpr Float inv2pi = 0.15915494309189533577f;
	constexpr Float inv4pi = 0.07957747154594766788f;
	constexpr Float piover2 = 1.57079632679489661923f;
	constexpr Float piover4 = 0.78539816339744830961f;
	constexpr Float sqrtsquare = 1.41421356237309504880f;

	// forward declaration
	template <typename T>
	class Vector2;
	template <typename T>
	class Vector3;
	template <typename T>
	class Point2;
	template <typename T>
	class Point3;
	template <typename T>
	class Normal3;
	template <typename T>
	class Bounds3;
	class Ray;
	class Texture;
	class RGBSpectrum;
	typedef RGBSpectrum Spectrum;
	class Ray;
	class Transform;
	class Intersection;
	class Shape;
	class Material;
	class Primitive;

	class ParamSet;
}

#endif // !ENG_H_
