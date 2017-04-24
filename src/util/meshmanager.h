/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MESH_MANAGER_H_
#define ORION_MESH_MANAGER_H_
#include <orion.h>
#include <common/singleton.h>
#include <shape/triangle.h>
#include <io/meshio.h>
namespace orion {

	//struct MeshBuffer
	//{
	//	int numTri, numVer;
	//	const int *vertexIndices;
	//	const Point3f *p;		// position
	//	const Normal3f *n;		// normal
	//	const Point2f *uv;		// uv
	//	MeshBuffer(int numTri, const int *vertexIndices, const Point3f *P,
	//		const Normal3f *N, const Point2f *UV) :numTri(numTri),
	//		vertexIndices(vertexIndices), p(P), n(N), uv(UV) {}
	//};

	class MeshManager : public Singleton<MeshManager>
	{
	private:
		std::map<std::string, std::shared_ptr<MeshData>> meshes;

	public:
		static void init() { new MeshManager(); }
		void clear();
		std::shared_ptr<MeshData> loadMesh(const std::string &name);

	private:
		MeshManager() {}
	};


}

#endif // !ORION_MESH_MANAGER_H_
