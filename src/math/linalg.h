/************************************************************************ 
 * @description :  线性代数基本函数
 * @author		:  Oscar Shen
 * @creat 		:  2017年4月18日10:22:33
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_LINALG_H_
#define ORION_LINALG_H_
#include <orion.h>
#include <core/geometry.h>
#include <util/logmanager.h>
namespace orion {
	// dot
	template <typename T>
	inline T dot(const Vector3<T> &v1, const Vector3<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	template <typename T>
	inline T dot(const Vector3<T> &v1, const Normal3<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	// cross
	template <typename T>
	inline Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2) {
		Float v1x = v1.x, v1y = v1.y, v1z = v1.z;
		Float v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}

	// length square
	template <typename T>
	inline T lengthSquare(const Vector3<T> &v) { return dot(v, v); }
	template <typename T>
	inline T lengthSquare(const Normal3<T> &v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

	// length
	template <typename T>
	inline T length(const Vector3<T> &v) { return std::sqrt(lengthSquare(v)); }
	template <typename T>
	inline T length(const Normal3<T> &v) { return std::sqrt(lengthSquare(v)); }

	// normalize
	template <typename T>
	inline Vector3<T> normalize(const Vector3<T> &v) { return v / length(v); }
	template <typename T>
	inline Normal3<T> normalize(const Normal3<T> &n) { return n / length(n); }

	// clamp between 0 and 1
	inline Float clamp(Float f) {
		return f < 0 ? 0 : (f > 1.0f ? 1.0f : f);
	}

	// clamp between _min_ and _max_
	inline Float clamp(Float f, Float min, Float max) {
		CHECK_INFO(min <= max, "clamp() : min should less than max!");
		return f < min ? min : (f > max ? max : f);
	}

	// inverse of 255, convenient to convert between float and char
	constexpr Float inv255 = 0.0039215686274509803921568627451f;

	// float to char
	inline char color_FloatToChar(Float f) {
		f = clamp(f);
		return (char)(f * 255);
	}

	// char to float
	inline Float color_CharToFloat(unsigned char c) {
		return c * inv255;
	}

	// degree to radians
	constexpr Float radians(Float degrees) {
		return degrees * static_cast<Float>(0.01745329251994329576923690768489);
	}
}

#endif // !ORT_LINALG_H_
