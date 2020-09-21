#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
{
	this->shaderProgram = shaderProgram;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->direction = direction;
}

void DirectionalLight::SetLightInShader()
{
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), direction.x, direction.y, direction.z);
}