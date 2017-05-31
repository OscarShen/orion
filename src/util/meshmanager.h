/************************************************************************ 
 * @description :  mesh resource manager
 * @author		:  Oscar Shen
 * @creat 		:  2017Äê4ÔÂ24ÈÕ
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

	class MeshManager : public Singleton<MeshManager>
	{
	private:
		std::map<std::string, std::shared_ptr<ModelData>> models;

	public:
		static void init() { new MeshManager(); }
		void clear();
		std::shared_ptr<ModelData> loadMeshData(const std::string &name);

	private:
		MeshManager() {}
	};
}

#endif // !ORION_MESH_MANAGER_H_
