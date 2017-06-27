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

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
ORION_NAMESPACE_BEGIN
// triangle mesh
struct Mesh
{
	// number of triangles, number of vertices
	int numTri, numVer;
	std::unique_ptr<Point3f[]> p;		// position
	std::unique_ptr<Normal3f[]> n;		// normal
	std::unique_ptr<Point2f[]> uv;		// uv

	void loadMesh(const std::string &filePath);
};

class Triangle
{
public:
	std::shared_ptr<Mesh> mesh;
	int id;

};



ORION_NAMESPACE_END

#endif // !ORION_CORE_TRIANGLE_H_
