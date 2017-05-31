#include "meshio.h"


namespace orion {

	void ModelLoader::loadModel(const std::string & path, ModelData &modeldata)
	{
		// Read file via Assimp
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			// TODO: Add to log!
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		processNode(modeldata, scene->mRootNode, scene);
	}

	void ModelLoader::processNode(ModelData &modeldata, aiNode * node, const aiScene * scene)
	{
		// Process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			MeshData mesh;
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
			procesMesh(mesh, aiMesh, scene);
			modeldata.push_back(mesh);
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(modeldata, node->mChildren[i], scene);
		}
	}

	void ModelLoader::procesMesh(MeshData &mesh, aiMesh * assimp_mesh, const aiScene * scene)
	{
		// 不使用 ASSIMP 自带的材质
		aiMaterial *material = scene->mMaterials[assimp_mesh->mMaterialIndex];
		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		mesh.matName = name.C_Str();
		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < assimp_mesh->mNumVertices; ++i)
		{
			// Positions
			mesh.vertices->push_back(Point3f(assimp_mesh->mVertices[i].x, assimp_mesh->mVertices[i].y, assimp_mesh->mVertices[i].z));
			// Normals
			if (assimp_mesh->mNormals)
				mesh.normals->push_back(Normal3f(assimp_mesh->mNormals[i].x, assimp_mesh->mNormals[i].y, assimp_mesh->mNormals[i].z));

			// Texture Coordinates
			if (assimp_mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				mesh.uvs->push_back(Point2f(assimp_mesh->mTextureCoords[0][i].x, assimp_mesh->mTextureCoords[0][i].y));
			}
		}
		mesh.num_vertices += assimp_mesh->mNumVertices;
		for (unsigned int i = 0; i < assimp_mesh->mNumFaces; ++i)
		{
			aiFace *face = assimp_mesh->mFaces + i;
			// Retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face->mNumIndices; ++j)
			{
				// Because all vertices in a vector, index of each vertex will overlap
				mesh.indices->push_back(face->mIndices[j]);
			}
		}
		mesh.num_triangles += assimp_mesh->mNumFaces;
	}
}
