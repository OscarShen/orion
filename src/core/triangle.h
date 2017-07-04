/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_TRIANGLE_H_
#define ORION_CORE_TRIANGLE_H_
#include <orion.h>
#include "geometry.h"
#include "transform.h"
ORION_NAMESPACE_BEGIN
// triangle mesh
struct Mesh
{
	// number of triangles, number of vertices
	int numTri, numVer;
	std::vector<Point3f> p;		// position
	std::vector<Normal3f> n;	// normal
	std::vector<Point2f> uv;	// uv

	void loadMesh(const std::string &filePath);
};

class Triangle
{
public:
	std::shared_ptr<Mesh> mesh;
	int triNumber;
	const Transform *local2world, *world2local;

	Triangle(const Transform *local2world, const Transform *world2local,
		const std::shared_ptr<Mesh> &mesh, int triNumber)
		: local2world(local2world), world2local(world2local), mesh(mesh),
		triNumber(triNumber) {}

	Bounds3f worldBound() const;
	Bounds3f localBound() const;

	Float area() const;

	Intersection sample(const Point2f &u, Float *pdf) const;
	Float pdf() const { return 1 / area(); }

	Intersection sample(const Intersection &ref, const Point2f &u, Float *pdf) const;

	// Calculate pdf in the case of we already get offset and intersect points,
	// _p_ means offset on this triangle.
	Float pdf(const Intersection &isec, const Vector3f &wi, const Scene &scene) const;
};

std::vector<std::shared_ptr<Triangle>> createTriangleMesh(const Transform *local2world, const Transform *world2local,
	std::shared_ptr<Mesh> meshdata);

ORION_NAMESPACE_END

#endif // !ORION_CORE_TRIANGLE_H_
