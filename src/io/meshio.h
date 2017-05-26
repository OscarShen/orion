/************************************************************************
* @project 		:  sloth
* @class   		:  ModelDataNM
* @version 		:  v1.0.0
* @description	:  记录包含法线贴图的模型数据结构
* @author		:  Oscar Shen
* @creat 		:  2017年2月21日21:34:41
* @revise 		:
************************************************************************
* Copyright @ OscarShen 2017. All rights reserved.
************************************************************************/
#pragma once
#ifndef ENG_MODEL_LOADER_H_
#define ENG_MODEL_LOADER_H_

#include <orion.h>
#include <core/geometry.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace orion {

	struct MeshData
	{
		std::shared_ptr<std::vector<Point3f>> vertices;
		std::shared_ptr<std::vector<Normal3f>> normals;
		std::shared_ptr<std::vector<Point2f>> uvs;
		std::shared_ptr<std::vector<int>> indices;
		int num_triangles;
		int num_vertices;
		MeshData() :vertices(new std::vector<Point3f>()), normals(new std::vector<Normal3f>()),
			uvs(new std::vector<Point2f>()), indices(new std::vector<int>()),
			num_triangles(0), num_vertices(0) {}
		MeshData(std::shared_ptr<std::vector<Point3f>> vertices,
			std::shared_ptr<std::vector<Normal3f>> normals,
			std::shared_ptr<std::vector<Point2f>> uvs,
			std::shared_ptr<std::vector<int>> indices,
			int num_triangles,
			int num_vertices) : vertices(vertices), normals(normals), uvs(uvs), indices(indices),
			num_triangles(num_triangles), num_vertices(num_vertices) {}
	};

	class ModelLoader
	{
	public:
		// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		static void loadModel(const std::string &path, MeshData &mesh);

	private:
		// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		static void processNode(MeshData &mesh, aiNode *node, const aiScene *scene);

		static void procesMesh(MeshData &mesh, aiMesh *assimp_mesh, const aiScene *scene);
	};
}


#endif // !SLOTH_MODEL_LOADER_H_
