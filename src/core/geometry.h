/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORT_GEOMETRY_H_
#define ORT_GEOMETRY_H_
#include <ort.h>
namespace ort {

	template <typename T>
	class Vector3
	{
	public:
		T x, y, z;
		Vector3() { x = y = z = 0; }
		Vector3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
		Vector3(T v) : x(v), y(v), z(v) {}

		Vector3(const Vector3<T> &v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		Vector3<T> &operator=(const Vector3<T> &v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		Vector3<T> operator+(const Vector3<T> &v) const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector3<T> &operator+=(const Vector3<T> &v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector3<T> operator-(const Vector3<T> &v) const {
			return Vector3(x - v.x, y - v.y, z - v.z);
		}
		Vector3<T> &operator-=(const Vector3<T> &v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		bool operator==(const Vector3<T> &v) const {
			return x == v.x && y == v.y && z == v.z;
		}

		bool operator!=(const Vector3<T> &v) const {
			return x != v.x || y != v.y || z != v.z;
		}

		template <typename U>
		Vector3<T> operator*(U s) const {
			return Vector3<T>(s * x, s * y, s * z);
		}

		template <typename U>
		Vector3<T> operator*(const Vector3<U> v) const {
			return Vector3<T>(v.x * x, v.y * y, v.z * z);
		}

		template <typename U>
		Vector3<T> &operator*=(U s) {
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}

		template <typename U>
		Vector3<T> &operator*=(const Vector3<U> v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		template <typename U>
		Vector3<T> operator/(U f) const {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			return Vector3<T>(x * inv, y * inv, z * inv);
		}

		template <typename U>
		Vector3<T> &operator/=(U f) {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}
		Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
	};

	template <typename T>
	inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}

	typedef Vector3<Float> Vector3f;
	typedef Vector3<int> Vector3i;

}

#endif // !ORT_GEOMETRY_H_
