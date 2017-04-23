/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_TRIANGLE_H_
#define ORION_TRIANGLE_H_
#include <orion.h>
#include <core/geometry.h>
#include "shape.h"
namespace orion {

	// TODO : Add transform
	struct TriangleMesh
	{
		// number of triangles, number of vertices
		int numTri, numVer;
		std::vector<int> vertexIndices;
		std::unique_ptr<Point3f[]> p;		// position
		std::unique_ptr<Normal3f[]> n;		// normal
		std::unique_ptr<Point2f[]> uv;		// uv

		TriangleMesh(int numTri,
			const int *vertexIndices, int numVer, const Point3f *P,
			const Normal3f *N, const Point2f *uv);
	};

	class Triangle : public Shape
	{
	private:
		std::shared_ptr<TriangleMesh> mesh;
		const int *v;

	public:
		Triangle(const std::shared_ptr<TriangleMesh> &mesh, int triNumber)
			:mesh(mesh) {
			v = &mesh->vertexIndices[3 * triNumber];
		}

		virtual bool intersect(const Ray &ray) const;
	};

}

#endif // !ORION_TRIANGLE_H_
