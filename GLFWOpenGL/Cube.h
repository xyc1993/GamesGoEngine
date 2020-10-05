#pragma once

#include "Mesh.h"

class Cube
{
public:
	Cube();
	Cube(Shader shader, glm::vec3 position, GLfloat angle, GLfloat scale);
	Cube(GLchar* diffusePath, GLchar* specularPath, Shader shader, glm::vec3 position, GLfloat angle, GLfloat scale);
	void Draw(Shader shader);
	void SetPosition(glm::vec3 position);

private:
	GLuint VAO, VBO;
	GLuint diffuseMap, specularMap;
	glm::vec3 position;
	GLfloat angle;
	GLfloat scale;
	bool applyTextures;

	void SetupCube();
	void ApplyTextures(GLchar* diffusePath, GLchar* specularPath, Shader shader);
};