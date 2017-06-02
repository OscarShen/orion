#include "transform.h"
#include "intersection.h"
#include <util/strutil.h>
namespace orion {
	Matrix4f::Matrix4f(Float mat[4][4]) { memcpy(m, mat, 16 * sizeof(Float)); }

	Matrix4f::Matrix4f(Float t00, Float t01, Float t02, Float t03, Float t10,
		Float t11, Float t12, Float t13, Float t20, Float t21,
		Float t22, Float t23, Float t30, Float t31, Float t32,
		Float t33) {
		m[0][0] = t00;
		m[0][1] = t01;
		m[0][2] = t02;
		m[0][3] = t03;
		m[1][0] = t10;
		m[1][1] = t11;
		m[1][2] = t12;
		m[1][3] = t13;
		m[2][0] = t20;
		m[2][1] = t21;
		m[2][2] = t22;
		m[2][3] = t23;
		m[3][0] = t30;
		m[3][1] = t31;
		m[3][2] = t32;
		m[3][3] = t33;
	}
	Matrix4f transpose(const Matrix4f &m)
	{
		return Matrix4f(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1],
			m.m[1][1], m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2],
			m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3],
			m.m[3][3]);
	}
	Matrix4f operator*(const Matrix4f & m1, const Matrix4f & m2)
	{
		Matrix4f r;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				r.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] +
				m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		return r;
	}
	Matrix4f inverse(const Matrix4f &m)
	{
		int indxc[4], indxr[4];
		int ipiv[4] = { 0, 0, 0, 0 };
		Float minv[4][4];
		memcpy(minv, m.m, 4 * 4 * sizeof(Float));
		for (int i = 0; i < 4; i++) {
			int irow = 0, icol = 0;
			Float big = 0.f;
			// Choose pivot
			for (int j = 0; j < 4; j++) {
				if (ipiv[j] != 1) {
					for (int k = 0; k < 4; k++) {
						if (ipiv[k] == 0) {
							if (std::abs(minv[j][k]) >= big) {
								big = Float(std::abs(minv[j][k]));
								irow = j;
								icol = k;
							}
						}
						else if (ipiv[k] > 1)
							CHECK_INFO(false, "Singular matrix in MatrixInvert");
					}
				}
			}
			++ipiv[icol];
			// Swap rows _irow_ and _icol_ for pivot
			if (irow != icol) {
				for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
			}
			indxr[i] = irow;
			indxc[i] = icol;
			if (minv[icol][icol] == 0.f) CHECK_INFO(false, "Singular matrix in MatrixInvert");

			// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
			Float pivinv = 1.f / minv[icol][icol];
			minv[icol][icol] = 1.;
			for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

			// Subtract this row from others to zero out their columns
			for (int j = 0; j < 4; j++) {
				if (j != icol) {
					Float save = minv[j][icol];
					minv[j][icol] = 0;
					for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
				}
			}
		}
		// Swap columns to reflect permutation
		for (int j = 3; j >= 0; j--) {
			if (indxr[j] != indxc[j]) {
				for (int k = 0; k < 4; k++)
					std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
			}
		}
		return Matrix4f(minv);
	}
	bool Transform::hasScale() const
	{
		Float la2 = (*this)(Vector3f(1, 0, 0)).lengthSquared();
		Float lb2 = (*this)(Vector3f(0, 1, 0)).lengthSquared();
		Float lc2 = (*this)(Vector3f(0, 0, 1)).lengthSquared();
#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
		return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
#undef NOT_ONE
	}
	Transform Transform::operator*(const Transform & t2) const
	{
		return Transform(m * t2.m, t2.mInv * mInv);
	}
	Intersection Transform::operator()(const Intersection & isec) const
	{
		Intersection ret;
		const Transform &t = *this;

		ret.pHit = t(isec.pHit);
		ret.n = normalize(t(isec.n));
		ret.dpdu = t(isec.dpdu);
		ret.dpdv = t(isec.dpdv);

		ret.t = isec.t;
		ret.primitive = isec.primitive;
		ret.uv = isec.uv;
		return ret;
	}
	Transform translate(const Vector3f & delta)
	{
		Matrix4f m(1, 0, 0, delta.x, 0, 1, 0, delta.y, 0, 0, 1, delta.z, 0, 0, 0,
			1);
		Matrix4f minv(1, 0, 0, -delta.x, 0, 1, 0, -delta.y, 0, 0, 1, -delta.z, 0,
			0, 0, 1);
		return Transform(m, minv);
	}
	Transform scale(Float x, Float y, Float z)
	{
		Matrix4f m(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
		Matrix4f minv(1 / x, 0, 0, 0, 0, 1 / y, 0, 0, 0, 0, 1 / z, 0, 0, 0, 0, 1);
		return Transform(m, minv);
	}
	Transform scale(const Vector3f &xyz)
	{
		return scale(xyz.x, xyz.y, xyz.z);
	}
	Transform rotateX(Float theta)
	{
		Float sinTheta = std::sin(radians(theta));
		Float cosTheta = std::cos(radians(theta));
		Matrix4f m(1, 0, 0, 0, 0, cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0,
			0, 0, 0, 1);
		return Transform(m, transpose(m));
	}
	Transform rotateY(Float theta)
	{
		Float sinTheta = std::sin(radians(theta));
		Float cosTheta = std::cos(radians(theta));
		Matrix4f m(cosTheta, 0, sinTheta, 0, 0, 1, 0, 0, -sinTheta, 0, cosTheta, 0,
			0, 0, 0, 1);
		return Transform(m, transpose(m));
	}
	Transform rotateZ(Float theta)
	{
		Float sinTheta = std::sin(radians(theta));
		Float cosTheta = std::cos(radians(theta));
		Matrix4f m(cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0, 0, 0, 0, 1, 0,
			0, 0, 0, 1);
		return Transform(m, transpose(m));
	}
	Transform rotate(Float theta, const Vector3f & axis)
	{
		Vector3f a = normalize(axis);
		Float sinTheta = std::sin(radians(theta));
		Float cosTheta = std::cos(radians(theta));
		Matrix4f m;
		// Compute rotation of first basis vector
		m.m[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
		m.m[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
		m.m[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
		m.m[0][3] = 0;

		// Compute rotations of second and third basis vectors
		m.m[1][0] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
		m.m[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
		m.m[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
		m.m[1][3] = 0;

		m.m[2][0] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
		m.m[2][1] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
		m.m[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
		m.m[2][3] = 0;
		return Transform(m, transpose(m));
	}
	Transform lookAt(const Point3f & pos, const Point3f & look, const Vector3f & up)
	{
		Vector3f f(normalize(look - pos));
		Vector3f s(normalize(cross(f, up)));
		Vector3f u(cross(s, f));

		Matrix4f result;
		result.m[0][0] = s.x;
		result.m[0][1] = s.y;
		result.m[0][2] = s.z;
		result.m[1][0] = u.x;
		result.m[1][1] = u.y;
		result.m[1][2] = u.z;
		result.m[2][0] = -f.x;
		result.m[2][1] = -f.y;
		result.m[2][2] = -f.z;
		result.m[0][3] = -dot(s, Vector3f(pos));
		result.m[1][3] = -dot(u, Vector3f(pos));
		result.m[2][3] = dot(f, Vector3f(pos));
		return Transform(inverse(result), result);	// local to world, so we need inverse _Matrix4f_
	}
	Transform createTransform(const ParamVec & param)
	{
		static std::vector<Transform> reuse(8); // for reuse transform

		Transform t;
		param.reset();
		const std::pair<std::string, std::string> *pair = nullptr;
		while (pair = param.getPair()) {
			if (pair->first == "store") {
				int pos = parseInt(pair->second);
				reuse[pos] = t;
			}
			else if (pair->first == "reuse") {
				int pos = parseInt(pair->second);
				t = reuse[pos] * t;
			}
			else if (pair->first == "scale") {
				Vector3f scaleVec = parseVector3f(pair->second);
				t = scale(scaleVec) * t;
			}
			else if (pair->first == "rotate") {
				Float degree;
				Vector3f rotateAxis = parseRotate(pair->second, degree);
				t = rotate(degree, rotateAxis) * t;
			}
			else if (pair->first == "translate") {
				Vector3f translateVec = parseVector3f(pair->second);
				t = translate(translateVec) * t;
			}
			else if (pair->first == "lookat") {
				t = parseLookAt(pair->second) * t;
			}
		}
		return t;
	}
}
