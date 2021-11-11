#pragma once

#include "Mesh.h"

class MeshBase
{
public:
	virtual void Draw() = 0;

protected:
	GLuint VAO, VBO;
};