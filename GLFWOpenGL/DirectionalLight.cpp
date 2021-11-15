#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{

}

DirectionalLight::DirectionalLight(GLuint lightNumber, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
{
	this->lightNumber = lightNumber;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->direction = direction;
}

void DirectionalLight::SetLightInShader(const GLuint& shaderProgram)
{
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".specular").c_str()), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".direction").c_str()), direction.x, direction.y, direction.z);
}

std::string DirectionalLight::GetNumberedShaderProperty()
{
	std::string number = std::to_string(lightNumber);
	return ("dirLight[" + number + "]").c_str();
}