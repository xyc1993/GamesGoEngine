#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class SubMesh
{
public:
	SubMesh(std::string name, std::vector<Vertex> vertices, std::vector<GLuint> indices);

	void Draw();
	void DrawInstanced(int meshCount);
	// useful for debugging mesh with several sub meshes
	std::string GetName() const;
	GLuint GetVAO() const;

private:
	void SetupMesh();

	std::string name;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, EBO;
};