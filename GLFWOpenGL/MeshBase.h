#pragma once

#include "Mesh.h"

class MeshBase
{
public:
	const GLuint& GetVAO() const;
	const GLuint& GetVBO() const;

protected:
	GLuint VAO, VBO;
};