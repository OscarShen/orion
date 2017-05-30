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

#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // min


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
	template <typename T>
	inline T dot(const Normal3<T> &v1, const Normal3<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	template <typename T>
	inline T dot(const Normal3<T> &v1, const Vector3<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	// absdot
	template <typename T>
	inline T absDot(const Vector3<T> &n1, const Vector3<T> &v2) {
		return std::abs(n1.x * v2.x + n1.y * v2.y + n1.z * v2.z);
	}
	template <typename T>
	inline T absDot(const Normal3<T> &n1, const Vector3<T> &v2) {
		return std::abs(n1.x * v2.x + n1.y * v2.y + n1.z * v2.z);
	}

	template <typename T>
	inline T absDot(const Vector3<T> &v1, const Normal3<T> &n2) {
		return std::abs(v1.x * n2.x + v1.y * n2.y + v1.z * n2.z);
	}

	template <typename T>
	inline T absDot(const Normal3<T> &n1, const Normal3<T> &n2) {
		return std::abs(n1.x * n2.x + n1.y * n2.y + n1.z * n2.z);
	}

	// cross
	template <typename T>
	inline Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2) {
		Float v1x = v1.x, v1y = v1.y, v1z = v1.z;
		Float v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}
	template <typename T>
	inline Vector3<T> cross(const Normal3<T> &v1, const Vector3<T> &v2) {
		Float v1x = v1.x, v1y = v1.y, v1z = v1.z;
		Float v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}
	template <typename T>
	inline Vector3<T> cross(const Vector3<T> &v1, const Normal3<T> &v2) {
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
	// clamp between _min_ and _max_
	inline int clamp(int f, int min, int max) {
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

	// degrees to radians
	constexpr Float radians(Float degrees) {
		return degrees * static_cast<Float>(0.01745329251994329576923690768489);
	}
	// radians to degrees
	constexpr Float degrees(Float radians) {
		return radians * static_cast<Float>(57.295779513082320876798154814105);

	}

	// solid angle algorithm
	inline Float cosTheta(const Vector3f &w) { return w.y; }
	inline Float cos2Theta(const Vector3f &w) { return w.y * w.y; }
	inline Float absCosTheta(const Vector3f &w) { return std::abs(w.y); }
	inline Float sin2Theta(const Vector3f &w) { return std::max(0.0f, 1.0f - cosTheta(w) * cosTheta(w)); }
	inline Float sinTheta(const Vector3f &w) { return std::sqrt(sin2Theta(w)); }
	inline Float cosPhi(const Vector3f &w) {
		Float sinTh = sinTheta(w);
		if (sinTh == 0) return 1.0f;
		return clamp(w.x / sinTh, -1.0f, 1.0f);
	}
	inline Float cos2Phi(const Vector3f &w) { return cosPhi(w) * cosPhi(w); }
	inline Float sinPhi(const Vector3f &w) {
		Float sinTh = sinTheta(w);
		if (sinTh == 0) return 0.0f;
		return clamp(-w.z / sinTh, -1.0f, 1.0f);
	}
	inline Float sin2Phi(const Vector3f &w) { return sinPhi(w) * sinPhi(w); }

	inline Float tanTheta(const Vector3f &w) { return sinTheta(w) / cosTheta(w); }
	inline Float tan2Theta(const Vector3f &w) { return sin2Theta(w) / cos2Theta(w); }

	inline bool sameHemisphere(const Vector3f &wi, const Vector3f &wo) { return wi.y * wo.y > 0; }
	inline Float sphericalTheta(const Vector3f &v) { return std::acos(clamp(v.y, -1.0f, 1.0f)); }
	inline Float sphericalPhi(const Vector3f &v) {
		Float p = std::atan2(-v.z, v.x);
		return (p < 0) ? p + 2 * pi : p;
	}

	template <typename T>
	inline Normal3<T> faceforward(const Normal3<T> &n, const Vector3<T> &v) {
		return (dot(n, v) < 0.f) ? -n : n;
	}
	template <typename T>
	inline Normal3<T> faceforward(const Normal3<T> &n, const Normal3<T> &v) {
		return (dot(n, v) < 0.f) ? -n : n;
	}

	template <typename T>
	inline void coordinateSystem(const Vector3<T> &y, Vector3<T> *z,
		Vector3<T> *x) {
		if (std::abs(y.x) > std::abs(y.y))
			*z = Vector3<T>(-y.z, 0, y.x) / std::sqrt(y.x * y.x + y.z * y.z);
		else
			*z = Vector3<T>(0, y.z, -y.y) / std::sqrt(y.y * y.y + y.z * y.z);
		*x = cross(y, *z);
	}

	inline Vector3f sphericalDirection(Float sinTheta, Float cosTheta, Float phi,
		const Vector3f &x, const Vector3f &y, const Vector3f &z) {
		return sinTheta * std::cos(phi) * x + cosTheta * y - sinTheta * std::sin(phi) * z;
	}

	// solve quadratic equation
	inline bool quadratic(Float A, Float B, Float C, Float *t0, Float *t1) {
		// Find quadratic discriminant
		double discrim = (double)B * (double)B - 4. * (double)A * (double)C;
		if (discrim < 0.) return false;
		double rootDiscrim = std::sqrt(discrim);

		// Compute quadratic _t_ values
		double q;
		if (B < 0)
			q = -.5 * (B - rootDiscrim);
		else
			q = -.5 * (B + rootDiscrim);
		*t0 = static_cast<Float>(q / A);
		*t1 = static_cast<Float>(C / q);
		if (*t0 > *t1) std::swap(*t0, *t1);
		return true;
	}

	template <typename Predicate>
	int findInterval(int size, const Predicate &pred) {
		int first = 0, length = size;
		while (length > 0) {
			int half = length >> 1, middle = first + half;
			if (pred(middle)) {
				first = middle + 1;
				length -= half + 1;
			}
			else
				length = half;
		}
		return clamp(first - 1, 0, size - 2);
	}
}

#endif // !ORT_LINALG_H_
