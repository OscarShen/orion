/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
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
#include <atomic>

#define ORION_NAMESPACE_BEGIN namespace orion {
#define ORION_NAMESPACE_END }

#if defined(_WIN32) || defined(_WIN64)
#define ORION_IN_WINDOWS
#elif defined(__linux__)
#define ORION_IN_LINUX
#endif

#define CHECK_INFO(x, str) if(!(x)) { std::cout << (str) << "\n\t|file: "<< __FILE__<<",line: " << __LINE__<<std::endl;}
#define CHECK_INFO(x, str) if(!(x)) { std::cout << (str) << "\n\t|file: "<< __FILE__<<",line: " << __LINE__<<std::endl;}
#define CHECK(x) CHECK_INFO(x, "check equal failed!")
#define CHECK_EQ(x, y) if((x)!=(y)) { std::cout << "check equal failed!" << "\n\t|file: "<< __FILE__<<",line: " << __LINE__<<std::endl; }
#define CHECK_NE(x, y) if((x)==(y)) { std::cout << "check not equal failed!" << "\n\t|file: "<< __FILE__<<",line: " << __LINE__<<std::endl; }

namespace orion {

typedef float Float;

constexpr Float epsilon = 0.00001;
constexpr Float shadowEpsilon = 0.01;

constexpr Float fInfinity = std::numeric_limits<Float>::max();
constexpr Float pi = 3.14159265358979323846f;
constexpr Float invpi = 0.31830988618379067154f;
constexpr Float inv2pi = 0.15915494309189533577f;
constexpr Float inv4pi = 0.07957747154594766788f;
constexpr Float piover2 = 1.57079632679489661923f;
constexpr Float piover4 = 0.78539816339744830961f;
constexpr Float sqrtsquare = 1.41421356237309504880f;

class Triangle;
class Primitive;
class Sampler;
class Camera;

ORION_NAMESPACE_END

#endif