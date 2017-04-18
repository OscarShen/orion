/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORT_LINALG_H_
#define ORT_LINALG_H_
#include <ort.h>
#include <core/geometry.h>
namespace ort {
	// dot
	template <typename T>
	inline T dot(const Vector3<T> &v1, const Vector3<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	// cross
	template <typename T>
	inline Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2) {
		double v1x = v1.x, v1y = v1.y, v1z = v1.z;
		double v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}

	// length square
	template <typename T>
	inline Float length_square(const Vector3<T> &v) { return dot(v, v); }

	// length
	template <typename T>
	inline Float length(const Vector3<T> &v) { return std::sqrt(length_square(v)); }

	template <typename T>
	inline Vector3<T> normalize(const Vector3<T> &v) { return v * (static_cast<T>(1) / length(v)); }
}

#endif // !ORT_LINALG_H_
