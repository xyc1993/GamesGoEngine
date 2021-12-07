#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshBase.h"
#include "SubMesh.h"

class MeshImported : public MeshBase
{
public:
	MeshImported(GLchar* path);

	virtual void DrawSubmesh(size_t submeshIndex) override;
	virtual int GetSubmeshesCount() const override;

	void ImportMesh(std::string path);

private:
	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void ProcessNode(aiNode* node, const aiScene* scene);
	SubMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<SubMesh*> subMeshes;
	std::string directory;
};