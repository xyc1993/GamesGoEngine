#pragma once

#include "Mesh.h"
#include "SharedAsset.h"

class MeshBase : public SharedAsset
{
public:
	virtual void Draw() = 0;

protected:
	GLuint VAO, VBO;
};
