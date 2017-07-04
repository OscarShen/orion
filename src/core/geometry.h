/************************************************************************
* @description :  定义基本几何变量
* @author		:  Oscar Shen
* @creat 		:  2017年4月18日10:22:12
************************************************************************
* Copyright @ OscarShen 2017. All rights reserved.
************************************************************************/
#pragma once
#ifndef ORION_GEOMETRY_H_
#define ORION_GEOMETRY_H_
#include <orion.h>
ORION_NAMESPACE_BEGIN

template <typename T>
class Vector2 {
public:
	T x, y;

	Vector2() { x = y = 0; }
	Vector2(T xx, T yy) : x(xx), y(yy) { }
	explicit Vector2(const Point2<T> &p) : x(p.x), y(p.y) { }
	explicit Vector2(const Point3<T> &p) : x(p.x), y(p.y) { }
	Vector2<T> operator+(const Vector2<T> &v) const {
		return Vector2(x + v.x, y + v.y);
	}

	Vector2<T> &operator+=(const Vector2<T> &v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2<T> operator-(const Vector2<T> &v) const {
		return Vector2(x - v.x, y - v.y);
	}

	Vector2<T> &operator-=(const Vector2<T> &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	bool operator==(const Vector2<T> &v) const { return x == v.x && y == v.y; }
	bool operator!=(const Vector2<T> &v) const { return x != v.x || y != v.y; }
	template <typename U>
	Vector2<T> operator*(U f) const {
		return Vector2<T>(f * x, f * y);
	}

	template <typename U>
	Vector2<T> &operator*=(U f) {
		x *= f;
		y *= f;
		return *this;
	}
	template <typename U>
	Vector2<T> operator/(U f) const {
		CHECK_NE(f, 0);
		Float inv = (Float)1 / f;
		return Vector2<T>(x * inv, y * inv);
	}

	template <typename U>
	Vector2<T> &operator/=(U f) {
		CHECK_NE(f, 0);
		Float inv = (Float)1 / f;
		x *= inv;
		y *= inv;
		return *this;
	}
	Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
	T operator[](int i) const {
		if (i == 0) return x;
		return y;
	}

	T &operator[](int i) {
		if (i == 0) return x;
		return y;
	}
	Float lengthSquared() const { return x * x + y * y; }
	Float length() const { return std::sqrt(lengthSquared()); }
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector2<T> &v) {
	os << "[ " << v.x << ", " << v.y << " ]";
	return os;
}

template <typename T>
class Vector3
{
public:
	T x, y, z;
	Vector3() { x = y = z = 0; }
	Vector3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	Vector3(T v) : x(v), y(v), z(v) {}
	explicit Vector3(const Point3<T> &p)
		: x(p.x), y(p.y), z(p.z) { }

	T operator[](int i) const {
		CHECK_INFO(i >= 0 && i <= 2, "index out of range");
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T &operator[](int i) {
		CHECK_INFO(i >= 0 && i <= 2, "index out of range");
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
	Float lengthSquared() const { return x * x + y * y + z * z; }
	Float length() const { return std::sqrt(lengthSquared()); }
	explicit Vector3(const Normal3<T> &n);
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
}

typedef Vector2<Float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector3<Float> Vector3f;
typedef Vector3<int> Vector3i;

template <typename T>
class Point2 {
public:

	T x, y;

	explicit Point2(const Point3<T> &p) : x(p.x), y(p.y) { }
	Point2() { x = y = 0; }
	Point2(T xx, T yy) : x(xx), y(yy) { }

	template <typename U>
	explicit Point2(const Point2<U> &p) {
		x = (T)p.x;
		y = (T)p.y;
	}

	template <typename U>
	explicit Point2(const Vector2<U> &p) {
		x = (T)p.x;
		y = (T)p.y;
	}

	template <typename U>
	explicit operator Vector2<U>() const {
		return Vector2<U>(x, y);
	}

	Point2<T> operator+(const Vector2<T> &v) const {
		return Point2<T>(x + v.x, y + v.y);
	}

	Point2<T> &operator+=(const Vector2<T> &v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2<T> operator-(const Point2<T> &p) const {
		return Vector2<T>(x - p.x, y - p.y);
	}

	Point2<T> operator-(const Vector2<T> &v) const {
		return Point2<T>(x - v.x, y - v.y);
	}
	Point2<T> operator-() const { return Point2<T>(-x, -y); }
	Point2<T> &operator-=(const Vector2<T> &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Point2<T> &operator+=(const Point2<T> &p) {
		x += p.x;
		y += p.y;
		return *this;
	}
	Point2<T> operator+(const Point2<T> &p) const {
		return Point2<T>(x + p.x, y + p.y);
	}
	template <typename U>
	Point2<T> operator*(U f) const {
		return Point2<T>(f * x, f * y);
	}
	template <typename U>
	Point2<T> &operator*=(U f) {
		x *= f;
		y *= f;
		return *this;
	}
	template <typename U>
	Point2<T> operator/(U f) const {
		CHECK_NE(f, 0);
		Float inv = (Float)1 / f;
		return Point2<T>(inv * x, inv * y);
	}
	template <typename U>
	Point2<T> &operator/=(U f) {
		CHECK_NE(f, 0);
		Float inv = (Float)1 / f;
		x *= inv;
		y *= inv;
		return *this;
	}
	T operator[](int i) const {
		if (i == 0) return x;
		return y;
	}

	T &operator[](int i) {
		if (i == 0) return x;
		return y;
	}
	bool operator==(const Point2<T> &p) const { return x == p.x && y == p.y; }
	bool operator!=(const Point2<T> &p) const { return x != p.x || y != p.y; }
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Point2<T> &p) {
	os << "[ " << p.x << ", " << p.y << " ]";
	return os;
}

template <typename T, typename U>
inline Point2<T> operator*(U f, const Point2<T> &p) {
	return p * f;
}

template <typename T>
class Point3 {
public:
	T x, y, z;

	Point3() { x = y = z = 0; }
	Point3(T x, T y, T z) : x(x), y(y), z(z) { }
	Point3(T v) { x = y = z = v; }
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

template <typename T, typename U>
inline Point3<T> operator*(U f, const Point3<T> &p) {
	return p * f;
}

typedef Point2<Float> Point2f;
typedef Point2<int> Point2i;
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
	Float lengthSquared() const { return x * x + y * y + z * z; }
	Float length() const { return std::sqrt(lengthSquared()); }
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Normal3<T> &v) {
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
}

typedef Normal3<Float> Normal3f;

template <typename T>
bool inside(const Point3<T> &p, const Bounds3<T> &b) {
	return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
		p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
}

template <typename T>
bool insideExclusive(const Point3<T> &p, const Bounds3<T> &b) {
	return (p.x >= b.pMin.x && p.x < b.pMax.x && p.y >= b.pMin.y &&
		p.y < b.pMax.y && p.z >= b.pMin.z && p.z < b.pMax.z);
}

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
	Point3<T> corner(int corner) const {
		return Point3<T>((*this)[(corner & 1)].x,
			(*this)[(corner & 2) ? 1 : 0].y,
			(*this)[(corner & 4) ? 1 : 0].z);
	}
	Vector3<T> diagonal() const { return pMax - pMin; }
	void boundingSphere(Point3<T> *center, Float *radius) const {
		*center = (pMin + pMax) / 2;
		*radius = inside(*center, *this) ? (*center - pMax).length() : 0;
	}
	T area() const {
		Vector3<T> d = diagonal();
		return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
	}
	T volume() const {
		Vector3<T> d = diagonal();
		return d.x * d.y * d.z;
	}
	int maxAxis() const {
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

	bool intersect(const Ray &ray, Float *hitt0 = nullptr, Float *hitt1 = nullptr) const;

	Vector3<T> offset(const Point3<T> &p) const {
		Vector3<T> o = p - pMin;
		if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
		if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
		if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
		return o;
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
	Float t, tMax;

	Ray() { }
	Ray(const Point3f &o, const Vector3f &d, Float t = 0.0f, Float tMax = fInfinity)
		: o(o), d(d), t(t), tMax(tMax) {}
	Point3f operator()(Float t) const { return o + d * t; }
	friend std::ostream &operator<<(std::ostream &os, const Ray &r) {
		os << "[o=" << r.o << ", d=" << r.d << "]";
		return os;
	}
};

// union is a key word, so use uppercase letter in place 
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

template<typename T>
inline bool Bounds3<T>::intersect(const Ray & ray, Float * hitt0, Float * hitt1) const
{
	Float t0 = 0, t1 = ray.tMax;
	for (int i = 0; i < 3; ++i) {
		Float invRayDir = 1 / ray.d[i];
		Float tNear = (pMin[i] - ray.o[i]) * invRayDir;
		Float tFar = (pMax[i] - ray.o[i]) * invRayDir;
		if (tNear > tFar) std::swap(tNear, tFar);
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if (t0 > t1) return false;
	}
	if (hitt0) *hitt0 = t0;
	if (hitt1) *hitt1 = t1;
	return true;
}

// expand with radius of _delta_ and center of _b_
template <typename T, typename U>
inline Bounds3<T> expand(const Bounds3<T> &b, U delta) {
	return Bounds3<T>(b.pMin - Vector3<T>(delta, delta, delta),
		b.pMax + Vector3<T>(delta, delta, delta));
}

template <typename T, typename U>
inline Vector3<T> operator*(U s, const Vector3<T> &v) {
	return v * s;
}

// return component of vector
template <typename T>
T maxComponent(const Vector3<T> &v) {
	return std::max(v.x, std::max(v.y, v.z));
}

// return dimension of vector, x=0, y=1, z=2
template <typename T>
int maxDimension(const Vector3<T> &v) {
	return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
}

// Arbitrary arrangement of the vector
template <typename T>
Vector3<T> permute(const Vector3<T> &v, int x, int y, int z) {
	return Vector3<T>(v[x], v[y], v[z]);
}

// Arbitrary arrangement of the point
template <typename T>
Point3<T> permute(const Point3<T> &p, int x, int y, int z) {
	return Point3<T>(p[x], p[y], p[z]);
}

// Abs of whole vector
template <typename T>
Vector3<T> abs(const Vector3<T> &v) {
	return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}
// Abs of whole vector
template <typename T>
Normal3<T> abs(const Normal3<T> &v) {
	return Normal3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}
template<typename T>
inline Vector3<T>::Vector3(const Normal3<T>& n)
	:x(n.x), y(n.y), z(n.z) {}

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
template <typename T>
inline Vector3<T> cross(const Normal3<T> &v1, const Normal3<T> &v2) {
	Float v1x = v1.x, v1y = v1.y, v1z = v1.z;
	Float v2x = v2.x, v2y = v2.y, v2z = v2.z;
	return Normal3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
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

inline Point3f offsetRayOrigin(const Point3f &p, const Normal3f &n, const Vector3f &w) {
	Float d = dot(abs(n), Vector3f(absDot(Vector3f(p), Vector3f(epsilon))));
	Vector3f offset = d * Vector3f(n);
	if (dot(w, n) < 0) offset = -offset;
	Point3f po = p + offset;
	return po;
}

// degrees to radians
constexpr Float radians(Float degrees) {
	return degrees * static_cast<Float>(0.01745329251994329576923690768489);
}
// radians to degrees
constexpr Float degrees(Float radians) {
	return radians * static_cast<Float>(57.295779513082320876798154814105);
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

template <typename T>
inline Normal3<T> faceforward(const Normal3<T> &n, const Normal3<T> &n2) {
	return (dot(n, n2) < 0.f) ? -n : n;
}

template <typename T>
inline void coordinateSystem(const Vector3<T> &z, Vector3<T> *x,
	Vector3<T> *y) {
	if (std::abs(z.x) > std::abs(z.y))
		*y = Vector3<T>(-z.z, 0, z.x) / std::sqrt(z.x * z.x + z.z * z.z);
	else
		*y = Vector3<T>(0, z.z, -z.y) / std::sqrt(z.y * z.y + z.z * z.z);
	*x = cross(*y, z);
}

ORION_NAMESPACE_END

#endif // !ORT_GEOMETRY_H_
