#include "triangle.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <sampler/sampling.h>
#include "scene.h"
ORION_NAMESPACE_BEGIN
void Mesh::loadMesh(const std::string & filePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	CHECK_INFO(ret, "Loading " + filePath + " failed");

	int size = (int) shapes.size();
	p.reset(new Point3f[size * 3]);
	n.reset(new Normal3f[size * 3]);
	uv.reset(new Point2f[size * 3]);

	int index = 0;
	for (int s = 0; s < size; s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			CHECK_INFO(fv == 3, "Only triangles supported");
			for (size_t v = 0; v < fv; ++v) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				float nx = attrib.normals[3 * idx.normal_index + 0];
				float ny = attrib.normals[3 * idx.normal_index + 1];
				float nz = attrib.normals[3 * idx.normal_index + 2];
				float tx = 0.0f, ty = 0.0f;
				if (idx.texcoord_index != -1) {
					tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				}
				p[index] = Point3f(vx, vy, vz);
				n[index] = Normal3f(nx, ny, nz);
				uv[index] = Point2f(tx, ty);
				++index;
			}
			index_offset += fv;
		}
		numTri = size;
		numVer = size * 3;
	}


}

Bounds3f Triangle::worldBound() const
{
	Point3f &p0 = mesh->p[triNumber * 3];
	Point3f &p1 = mesh->p[triNumber * 3 + 1];
	Point3f &p2 = mesh->p[triNumber * 3 + 2];
	return Union(Bounds3f(p0, p1), p2);
}

Bounds3f Triangle::localBound() const
{
	Point3f &p0 = mesh->p[triNumber * 3];
	Point3f &p1 = mesh->p[triNumber * 3 + 1];
	Point3f &p2 = mesh->p[triNumber * 3 + 2];
	return Union(Bounds3f((*world2local)(p0), (*world2local)(p1)),
		(*world2local)(p2));
}

Float Triangle::area() const
{
	Point3f &p0 = mesh->p[triNumber * 3];
	Point3f &p1 = mesh->p[triNumber * 3 + 1];
	Point3f &p2 = mesh->p[triNumber * 3 + 2];
	return 0.5f * cross(p1 - p0, p2 - p0).length();
}

Intersection Triangle::sample(const Point2f & u, Float * pdf) const
{
	Point2f b = uniformSampleTriangle(u);

	Point3f &p0 = mesh->p[triNumber * 3];
	Point3f &p1 = mesh->p[triNumber * 3 + 1];
	Point3f &p2 = mesh->p[triNumber * 3 + 2];

	Point3f p = b[0] * p0 + b[1] * p1 + (1 - b[0] - b[1]) * p2;
	Normal3f ng = Normal3f(normalize(cross(p1 - p0, p2 - p0)));
	Normal3f ns = mesh->n[triNumber * 3    ] * b[0] +
				  mesh->n[triNumber * 3 + 1] * b[1] +
				  mesh->n[triNumber * 3 + 2] * (1.0f - b[0] - b[1]);
	ng = faceforward(ng, ns);

	Intersection isec;
	isec.p = p;
	isec.ng = ng;
	isec.ns = ns;
	*pdf = 1.0f / area();
	return isec;
}

Intersection Triangle::sample(const Intersection & ref, const Point2f & u, Float * pdf) const
{
	Intersection isec = sample(u, pdf);
	Vector3f wi = isec.p - ref.p;
	if (wi.lengthSquared() == 0)
		*pdf = 0;
	else {
		wi = normalize(wi);
		*pdf *= (ref.p - isec.p).lengthSquared() / absDot(isec.ng, -wi);
		if (std::isinf(*pdf))
			*pdf = 0;
	}
	return isec;
}

Float Triangle::pdf(const Intersection & isec, const Vector3f & wi, const Scene & scene) const
{
	Ray ray = isec.spawnRay(wi);
	Float t;
	Intersection in;
	if (!scene.intersect(ray, &in) || in.primitive->triangle.get() != this)
		return 0;

	Float pdf = (isec.p - in.p).lengthSquared() /
		(absDot(in.ng, -wi) * area());
	if (std::isinf(pdf))
		pdf = 0;
	return pdf;
}

ORION_NAMESPACE_END

