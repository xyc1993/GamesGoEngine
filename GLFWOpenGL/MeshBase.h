#pragma once

#include "SubMesh.h"
#include "SharedAsset.h"

class MeshBase : public SharedAsset
{
public:
	virtual void DrawSubMesh(size_t subMeshIndex) = 0;
	virtual int GetSubMeshesCount() const = 0;

protected:
	GLuint VAO, VBO;
};