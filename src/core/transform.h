/************************************************************************ 
 * @description :  transform
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-22 11:56:19
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_TRANSFORM_H_
#define ORION_TRANSFORM_H_
#include <orion.h>
#include <common/paramset.h>
#include <math/linalg.h>
#include "geometry.h"
namespace orion {

	struct Matrix4f {

		Float m[4][4];

		Matrix4f() {
			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
			m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] =
				m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.f;
		}
		Matrix4f(Float mat[4][4]);
		Matrix4f(Float t00, Float t01, Float t02, Float t03, Float t10, Float t11,
			Float t12, Float t13, Float t20, Float t21, Float t22, Float t23,
			Float t30, Float t31, Float t32, Float t33);
		bool operator==(const Matrix4f &m2) const {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					if (m[i][j] != m2.m[i][j]) return false;
			return true;
		}
		bool operator!=(const Matrix4f &m2) const {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					if (m[i][j] != m2.m[i][j]) return true;
			return false;
		}
		friend Matrix4f operator*(const Matrix4f &m1, const Matrix4f &m2);

		// Get transpose of matrix
		friend Matrix4f transpose(const Matrix4f &m);

		// Get inverse of matrix
		friend Matrix4f inverse(const Matrix4f &m);

		friend std::ostream &operator<<(std::ostream &os, const Matrix4f &m) {
			os << "[ [ " << m.m[0][0] << ", " << m.m[0][1] << ", " << m.m[0][2] << ", " << m.m[0][3] << " ] "
				 << "[ " << m.m[1][0] << ", " << m.m[1][1] << ", " << m.m[1][2] << ", " << m.m[1][3] << " ] "
				 << "[ " << m.m[2][0] << ", " << m.m[2][1] << ", " << m.m[2][2] << ", " << m.m[2][3] << " ] "
				 << "[ " << m.m[3][0] << ", " << m.m[3][1] << ", " << m.m[3][2] << ", " << m.m[3][3] << " ] ]";
			return os;
		}
	};

	class Transform
	{
	private:
		Matrix4f m, mInv;

	public:
		Transform() {}
		Transform(const Matrix4f &m) : m(m), mInv(inverse(m)) {}
		Transform(const Matrix4f &m, const Matrix4f &mInv) : m(m), mInv(mInv) {}

		friend Transform inverse(const Transform &t) {
			return Transform(t.mInv, t.m);
		}
		bool operator==(const Transform &t) const {
			return t.m == m && t.mInv == mInv;
		}
		bool operator!=(const Transform &t) const {
			return t.m != m || t.mInv != mInv;
		}
		bool isIdentity() const {
			return (m.m[0][0] == 1.f && m.m[0][1] == 0.f && m.m[0][2] == 0.f &&
				m.m[0][3] == 0.f && m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
				m.m[1][2] == 0.f && m.m[1][3] == 0.f && m.m[2][0] == 0.f &&
				m.m[2][1] == 0.f && m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
				m.m[3][0] == 0.f && m.m[3][1] == 0.f && m.m[3][2] == 0.f &&
				m.m[3][3] == 1.f);
		}
		const Matrix4f &getMatrix() const { return m; }
		const Matrix4f &getInverseMatrix() const { return mInv; }
		bool hasScale() const;

		template <typename T>
		inline Point3<T> operator()(const Point3<T> &p) const;
		template <typename T>
		inline Vector3<T> operator()(const Vector3<T> &v) const;
		template <typename T>
		inline Normal3<T> operator()(const Normal3<T> &n) const;
		inline Ray operator()(const Ray &r) const;
		Bounds3f operator()(const Bounds3f &b) const;
		Transform operator*(const Transform &t2) const;
		bool operator<(const Transform &t2) const {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j) {
					if (m.m[i][j] < t2.m.m[i][j]) return true;
					if (m.m[i][j] > t2.m.m[i][j]) return false;
				}
			return false;
		}
		friend std::ostream &operator<<(std::ostream &os, const Transform &t) {
			os << "t=" << t.m << ", inv=" << t.mInv;
			return os;
		}
	};	

	Transform translate(const Vector3f &delta);
	Transform scale(Float x, Float y, Float z);
	Transform scale(const Vector3f &xyz);
	Transform rotateX(Float theta);
	Transform rotateY(Float theta);
	Transform rotateZ(Float theta);
	Transform rotate(Float theta, const Vector3f &axis);
	// camera to world
	Transform lookAt(const Point3f &pos, const Point3f &look, const Vector3f &up);

	template<typename T>
	inline Point3<T> Transform::operator()(const Point3<T>& p) const
	{
		T x = p.x, y = p.y, z = p.z;
		T xp = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
		T yp = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
		T zp = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
		T wp = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
		CHECK_NE(wp, 0);
		if (wp == 1)
			return Point3<T>(xp, yp, zp);
		else
			return Point3<T>(xp, yp, zp) / wp;
	}
	template<typename T>
	inline Vector3<T> Transform::operator()(const Vector3<T>& v) const
	{
		T x = v.x, y = v.y, z = v.z;
		return Vector3<T>(m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z,
			m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z,
			m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z);
	}
	inline Ray orion::Transform::operator()(const Ray & r) const
	{
		Point3f o = (*this)(r.o);
		Vector3f d = (*this)(r.d);
		return Ray(o, d, r.depth, r.time, r.tMax);
	}
	inline Bounds3f Transform::operator()(const Bounds3f & b) const
	{
		const Transform &M = *this;
		Bounds3f ret(M(Point3f(b.pMin.x, b.pMin.y, b.pMin.z)));
		ret = Union(ret, M(Point3f(b.pMax.x, b.pMin.y, b.pMin.z)));
		ret = Union(ret, M(Point3f(b.pMin.x, b.pMax.y, b.pMin.z)));
		ret = Union(ret, M(Point3f(b.pMin.x, b.pMin.y, b.pMax.z)));
		ret = Union(ret, M(Point3f(b.pMin.x, b.pMax.y, b.pMax.z)));
		ret = Union(ret, M(Point3f(b.pMax.x, b.pMax.y, b.pMin.z)));
		ret = Union(ret, M(Point3f(b.pMax.x, b.pMin.y, b.pMax.z)));
		ret = Union(ret, M(Point3f(b.pMax.x, b.pMax.y, b.pMax.z)));
		return ret;
	}
	template<typename T>
	inline Normal3<T> Transform::operator()(const Normal3<T>&n) const
	{
		T x = n.x, y = n.y, z = n.z;
		return Normal3<T>(mInv.m[0][0] * x + mInv.m[1][0] * y + mInv.m[2][0] * z,
			mInv.m[0][1] * x + mInv.m[1][1] * y + mInv.m[2][1] * z,
			mInv.m[0][2] * x + mInv.m[1][2] * y + mInv.m[2][2] * z);
	}


	Transform createTransform(const ParamVec &param);
}
#endif // !ORION_TRANSFORM_H_
