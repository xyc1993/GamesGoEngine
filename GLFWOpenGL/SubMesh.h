#pragma once

#include "MeshOld.h"

class SubMesh
{
public:
	SubMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);

	void Draw();

private:
	void SetupMesh();

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, EBO;
};