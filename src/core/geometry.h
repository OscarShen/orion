/************************************************************************ 
 * @description :  定义基本几何变量
 * @author		:  Oscar Shen
 * @creat 		:  2017年4月18日10:22:12
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

		T operator[](int i) const {
			CHECK(i >= 0 && i <= 2);
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}
		T &operator[](int i) {
			CHECK(i >= 0 && i <= 2);
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
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

	template <typename T>
	class Point3 {
	public:
		T x, y, z;

		Point3() { x = y = z = 0; }
		Point3(T x, T y, T z) : x(x), y(y), z(z) { }
		template <typename U>
		explicit Point3(const Point3<U> &p)
			: x((T)p.x), y((T)p.y), z((T)p.z) {
		}
		template <typename U>
		explicit operator Vector3<U>() const {
			return Vector3<U>(x, y, z);
		}

		Point3<T> operator+(const Vector3<T> &v) const {
			return Point3<T>(x + v.x, y + v.y, z + v.z);
		}

		Point3<T> &operator+=(const Vector3<T> &v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		Vector3<T> operator-(const Point3<T> &p) const {
			return Vector3<T>(x - p.x, y - p.y, z - p.z);
		}
		Point3<T> operator-(const Vector3<T> &v) const {
			return Point3<T>(x - v.x, y - v.y, z - v.z);
		}
		Point3<T> &operator-=(const Vector3<T> &v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		Point3<T> &operator+=(const Point3<T> &p) {
			x += p.x;
			y += p.y;
			z += p.z;
			return *this;
		}
		Point3<T> operator+(const Point3<T> &p) const {
			return Point3<T>(x + p.x, y + p.y, z + p.z);
		}
		template <typename U>
		Point3<T> operator*(U f) const {
			return Point3<T>(f * x, f * y, f * z);
		}
		template <typename U>
		Point3<T> &operator*=(U f) {
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		template <typename U>
		Point3<T> operator/(U f) const {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			return Point3<T>(inv * x, inv * y, inv * z);
		}
		template <typename U>
		Point3<T> &operator/=(U f) {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}
		T operator[](int i) const {
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		T &operator[](int i) {
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}
		bool operator==(const Point3<T> &p) const {
			return x == p.x && y == p.y && z == p.z;
		}
		bool operator!=(const Point3<T> &p) const {
			return x != p.x || y != p.y || z != p.z;
		}
		Point3<T> operator-() const { return Point3<T>(-x, -y, -z); }
	};

	template <typename T>
	inline std::ostream &operator<<(std::ostream &os, const Point3<T> &v) {
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}

	typedef Point3<Float> Point3f;
	typedef Point3<int> Point3i;

	template <typename T>
	class Normal3 {
	public:
		T x, y, z;
		Normal3() { x = y = z = 0; }
		Normal3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
		Normal3<T> operator-() const { return Normal3(-x, -y, -z); }
		Normal3<T> operator+(const Normal3<T> &n) const {
			return Normal3<T>(x + n.x, y + n.y, z + n.z);
		}

		Normal3<T> &operator+=(const Normal3<T> &n) {
			x += n.x;
			y += n.y;
			z += n.z;
			return *this;
		}
		Normal3<T> operator-(const Normal3<T> &n) const {
			return Normal3<T>(x - n.x, y - n.y, z - n.z);
		}

		Normal3<T> &operator-=(const Normal3<T> &n) {
			x -= n.x;
			y -= n.y;
			z -= n.z;
			return *this;
		}
		template <typename U>
		Normal3<T> operator*(U f) const {
			return Normal3<T>(f * x, f * y, f * z);
		}

		template <typename U>
		Normal3<T> &operator*=(U f) {
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		template <typename U>
		Normal3<T> operator/(U f) const {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			return Normal3<T>(x * inv, y * inv, z * inv);
		}

		template <typename U>
		Normal3<T> &operator/=(U f) {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}

		explicit Normal3<T>(const Vector3<T> &v) : x(v.x), y(v.y), z(v.z) {
		}
		bool operator==(const Normal3<T> &n) const {
			return x == n.x && y == n.y && z == n.z;
		}
		bool operator!=(const Normal3<T> &n) const {
			return x != n.x || y != n.y || z != n.z;
		}

		T operator[](int i) const {
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		T &operator[](int i) {
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}
	};

	template <typename T>
	inline std::ostream &operator<<(std::ostream &os, const Normal3<T> &v) {
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}

	typedef Normal3<Float> Normal3f;

	template <typename T>
	class Bounds3 {
	public:
		Point3<T> pMin, pMax;

		Bounds3() {
			T minNum = std::numeric_limits<T>::lowest();
			T maxNum = std::numeric_limits<T>::max();
			pMin = Point3<T>(maxNum, maxNum, maxNum);
			pMax = Point3<T>(minNum, minNum, minNum);
		}
		explicit Bounds3(const Point3<T> &p) : pMin(p), pMax(p) {}
		Bounds3(const Point3<T> &p1, const Point3<T> &p2)
			: pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
				std::min(p1.z, p2.z)),
			pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
				std::max(p1.z, p2.z)) {}
		const Point3<T> &operator[](int i) const {
			return (i == 0) ? pMin : pMax;
		}
		Point3<T> &operator[](int i) {
			return (i == 0) ? pMin : pMax;
		}
		bool operator==(const Bounds3<T> &b) const {
			return b.pMin == pMin && b.pMax == pMax;
		}
		bool operator!=(const Bounds3<T> &b) const {
			return b.pMin != pMin || b.pMax != pMax;
		}
		Point3<T> Corner(int corner) const {
			return Point3<T>((*this)[(corner & 1)].x,
				(*this)[(corner & 2) ? 1 : 0].y,
				(*this)[(corner & 4) ? 1 : 0].z);
		}
		Vector3<T> diagonal() const { return pMax - pMin; }
		T area() const {
			Vector3<T> d = diagonal();
			return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
		}
		T volume() const {
			Vector3<T> d = diagonal();
			return d.x * d.y * d.z;
		}
		int max_axis() const {
			Vector3<T> d = diagonal();
			if (d.x > d.y && d.x > d.z)
				return 0;
			else if (d.y > d.z)
				return 1;
			else
				return 2;
		}
		template <typename U>
		explicit operator Bounds3<U>() const {
			return Bounds3<U>((Point3<U>)pMin, (Point3<U>)pMax);
		}
		// delta : some loosen to avoid float error
		bool inside(const Point3<T> &p, Float delta) {
			if (p.x > pMax.x + delta || p.x < pMin.x - delta)
				return false;
			if (p.y > pMax.y + delta || p.y < pMin.y - delta)
				return false;
			if (p.z > pMax.z + delta || p.z < pMin.z - delta)
				return false;

			return true;
		}
		friend std::ostream &operator<<(std::ostream &os, const Bounds3<T> &b) {
			os << "[ " << b.pMin << " - " << b.pMax << " ]";
			return os;
		}
	};

	typedef Bounds3<Float> Bounds3f;
	typedef Bounds3<int> Bounds3i;

	class Ray {
	public:
		Point3f o;
		Vector3f d;

		Ray() {}
		Ray(const Point3f &o, const Vector3f &d) : o(o), d(d) {}
		Point3f operator()(Float t) const { return o + d * t; }
		friend std::ostream &operator<<(std::ostream &os, const Ray &r) {
			os << "[o=" << r.o << ", d=" << r.d << "]";
			return os;
		}
	};

	// union is a key word
	template <typename T>
	Bounds3<T> Union(const Bounds3<T> &b, const Point3<T> &p) {
		return Bounds3<T>(
			Point3<T>(std::min(b.pMin.x, p.x), std::min(b.pMin.y, p.y),
				std::min(b.pMin.z, p.z)),
			Point3<T>(std::max(b.pMax.x, p.x), std::max(b.pMax.y, p.y),
				std::max(b.pMax.z, p.z)));
	}

	template <typename T>
	Bounds3<T> Union(const Bounds3<T> &b1, const Bounds3<T> &b2) {
		return Bounds3<T>(Point3<T>(std::min(b1.pMin.x, b2.pMin.x),
			std::min(b1.pMin.y, b2.pMin.y),
			std::min(b1.pMin.z, b2.pMin.z)),
			Point3<T>(std::max(b1.pMax.x, b2.pMax.x),
				std::max(b1.pMax.y, b2.pMax.y),
				std::max(b1.pMax.z, b2.pMax.z)));
	}

}

#endif // !ORT_GEOMETRY_H_
