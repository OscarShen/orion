#include "triangle.h"

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
ORION_NAMESPACE_END
