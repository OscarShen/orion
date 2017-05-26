#include "triangle.h"
#include <sampler/sampling.h>
namespace orion {

	TriangleMesh::TriangleMesh(const Transform &local2world, const std::shared_ptr<MeshData> &meshdata)
		: numTri(meshdata->num_triangles), numVer(meshdata->num_vertices),
		vertexIndices(meshdata->indices->data(), meshdata->indices->data() + 3 * numTri)
	{
		// vertices
		Point3f *P = meshdata->vertices->data();
		p.reset(new Point3f[numVer]);
		for (int i = 0; i < numVer; ++i) p[i] = local2world(P[i]);

		// uv
		Point2f *UV = meshdata->uvs == nullptr ? nullptr : meshdata->uvs->data();
		if (UV) {
			uv.reset(new Point2f[numVer]);
			memcpy(uv.get(), UV, numVer * sizeof(Point2f));
		}

		// normal
		Normal3f *N = meshdata->normals == nullptr ? nullptr : meshdata->normals->data();
		if (N) {
			n.reset(new Normal3f[numVer]);
			for (int i = 0; i < numVer; ++i) n[i] = local2world(N[i]);
		}
	}

	bool Triangle::intersect(const Ray & ray, Intersection *isec) const
	{
		// Get triangle vertices in _p0_, _p1_, and _p2_
		const Point3f &p0 = mesh->p[v[0]];
		const Point3f &p1 = mesh->p[v[1]];
		const Point3f &p2 = mesh->p[v[2]];

		// Perform ray--triangle intersection test

		// Transform triangle vertices to ray coordinate space

		// Translate vertices based on ray origin
		Point3f p0t = p0 - Vector3f(ray.o);
		Point3f p1t = p1 - Vector3f(ray.o);
		Point3f p2t = p2 - Vector3f(ray.o);

		// Permute components of triangle vertices and ray direction
		int kz = maxDimension(abs(ray.d));
		int kx = kz + 1;
		if (kx == 3) kx = 0;
		int ky = kx + 1;
		if (ky == 3) ky = 0;
		Vector3f d = permute(ray.d, kx, ky, kz);
		p0t = permute(p0t, kx, ky, kz);
		p1t = permute(p1t, kx, ky, kz);
		p2t = permute(p2t, kx, ky, kz);

		// Apply shear transformation to translated vertex positions
		Float Sx = -d.x / d.z;
		Float Sy = -d.y / d.z;
		Float Sz = 1.f / d.z;
		p0t.x += Sx * p0t.z;
		p0t.y += Sy * p0t.z;
		p1t.x += Sx * p1t.z;
		p1t.y += Sy * p1t.z;
		p2t.x += Sx * p2t.z;
		p2t.y += Sy * p2t.z;

		// Compute edge function coefficients _e0_, _e1_, and _e2_
		Float e0 = p1t.x * p2t.y - p1t.y * p2t.x;
		Float e1 = p2t.x * p0t.y - p2t.y * p0t.x;
		Float e2 = p0t.x * p1t.y - p0t.y * p1t.x;

		// Fall back to double precision test at triangle edges
		if (sizeof(Float) == sizeof(float) &&
			(e0 == 0.0f || e1 == 0.0f || e2 == 0.0f)) {
			double p2txp1ty = (double)p2t.x * (double)p1t.y;
			double p2typ1tx = (double)p2t.y * (double)p1t.x;
			e0 = (float)(p2typ1tx - p2txp1ty);
			double p0txp2ty = (double)p0t.x * (double)p2t.y;
			double p0typ2tx = (double)p0t.y * (double)p2t.x;
			e1 = (float)(p0typ2tx - p0txp2ty);
			double p1txp0ty = (double)p1t.x * (double)p0t.y;
			double p1typ0tx = (double)p1t.y * (double)p0t.x;
			e2 = (float)(p1typ0tx - p1txp0ty);
		}

		// Perform triangle edge and determinant tests
		if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
			return false;
		Float det = e0 + e1 + e2;
		if (det == 0) return false;

		// Compute scaled hit distance to triangle and test against ray $t$ range
		p0t.z *= Sz;
		p1t.z *= Sz;
		p2t.z *= Sz;
		Float tScaled = e0 * p0t.z + e1 * p1t.z + e2 * p2t.z;
		if (det < 0 && (tScaled >= 0 || tScaled < ray.tMax * det))
			return false;
		else if (det > 0 && (tScaled <= 0 || tScaled > ray.tMax * det))
			return false;

		// Compute barycentric coordinates and $t$ value for triangle intersection
		Float invDet = 1 / det;
		Float b0 = e0 * invDet;
		Float b1 = e1 * invDet;
		Float b2 = e2 * invDet;
		Float t = tScaled * invDet;

		// Compute triangle partial derivatives
		Vector3f dpdu, dpdv;
		Point2f uv[3];
		_getUVs(uv);

		// Compute deltas for triangle partial derivatives
		Vector2f duv02 = uv[0] - uv[2], duv12 = uv[1] - uv[2];
		Vector3f dp02 = p0 - p2, dp12 = p1 - p2;
		Float determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];
		Float invdet = 1 / determinant;
		dpdu = ( duv12[1] * dp02 - duv02[1] * dp12) * invdet;
		dpdv = (-duv12[0] * dp02 + duv02[0] * dp12) * invdet;

		Point3f pHit = p0 * b0 + p1 * b1 + p2 * b2;
		Point2f uvHit = uv[0] * b0 + uv[1] * b1 + uv[2] * b2;

		*isec = Intersection(pHit, Normal3f(0), uvHit, t);
		isec->dpdu = dpdu;
		isec->dpdv = dpdv;

		// get shading normal, also has shading tangent in future
		if (mesh->n) {
			Normal3f n[3];
			_getNormals(n);
			Normal3f ns(n[0] * b0 + n[1] * b1 + n[2] * b2);
			if (ns.lengthSquared() > 0)
				isec->n = faceforward(normalize(ns), -ray.d);
		}
		else {
			Normal3f n(normalize(cross(dp02, dp12)));
			if (n.lengthSquared() > 0)
				isec->n = faceforward(normalize(n), -ray.d);
		}
		return true;
	}

	Bounds3f Triangle::worldBound() const
	{
		// default in world coordination
		auto &p0 = mesh->p[v[0]];
		auto &p1 = mesh->p[v[1]];
		auto &p2 = mesh->p[v[2]];
		return Union(Bounds3f(p0, p1), p2);
	}

	Bounds3f Triangle::localBound() const
	{
		const Point3f &p0 = mesh->p[v[0]];
		const Point3f &p1 = mesh->p[v[1]];
		const Point3f &p2 = mesh->p[v[2]];
		return Union(Bounds3f((*world2local)(p0), (*world2local)(p1)),
			(*world2local)(p2));
	}

	Float Triangle::area() const
	{
		const Point3f &p0 = mesh->p[v[0]];
		const Point3f &p1 = mesh->p[v[1]];
		const Point3f &p2 = mesh->p[v[2]];
		return 0.5f * cross(p1 - p0, p2 - p0).length();
	}

	Intersection Triangle::sample(const Point2f & u, Float * pdf) const
	{
		Point2f b = uniformSampleTriangle(u);

		const Point3f &p0 = mesh->p[v[0]];
		const Point3f &p1 = mesh->p[v[1]];
		const Point3f &p2 = mesh->p[v[2]];
		Intersection isec;
		isec.pHit = p0 * b[0] + p1 * b[1] + p2 * (1 - b[0] - b[1]);
		isec.n = normalize(Normal3f(cross(p2 - p0, p1 - p0))); // right hand
		if (mesh->n) {
			Normal3f ns(mesh->n[v[0]] * b[0] + mesh->n[v[1]] * b[1] + mesh->n[v[2]] * (1 - b[0] - b[1]));
			isec.n = faceforward(isec.n, ns);
		}
		*pdf = 1 / area();
		return isec;
	}

	void Triangle::_getUVs(Point2f uv[3]) const
	{
		if (mesh->uv != nullptr) {
			uv[0] = mesh->uv[v[0]];
			uv[1] = mesh->uv[v[1]];
			uv[2] = mesh->uv[v[2]];
		}
		else {
			uv[0] = Point2f(0, 0);
			uv[1] = Point2f(1, 0);
			uv[2] = Point2f(1, 1);
		}
	}

	void Triangle::_getNormals(Normal3f n[3]) const
	{
		// call this function means must have normal vector
		if (mesh->n != nullptr) {
			n[0] = mesh->n[v[0]];
			n[1] = mesh->n[v[1]];
			n[2] = mesh->n[v[2]];
		}
	}


	std::vector<std::shared_ptr<Shape>> createTriangleMesh(const Transform *local2world,
		const Transform *world2local, const std::shared_ptr<MeshData> &meshdata)
	{
		std::shared_ptr<TriangleMesh> mesh(new TriangleMesh(*local2world, meshdata));
		std::vector<std::shared_ptr<Shape>> tris;
		tris.reserve(mesh->numTri);
		for (int i = 0; i < mesh->numTri; ++i) {
			tris.push_back(std::make_shared<Triangle>(local2world, world2local, mesh, i));
		}
		return tris;
	}

}
