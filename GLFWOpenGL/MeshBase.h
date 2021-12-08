#pragma once

#include "SubMesh.h"
#include "SharedAsset.h"

class MeshBase : public SharedAsset
{
public:
	virtual void DrawSubmesh(size_t submeshIndex) = 0;
	virtual int GetSubmeshesCount() const = 0;

protected:
	GLuint VAO, VBO;
};