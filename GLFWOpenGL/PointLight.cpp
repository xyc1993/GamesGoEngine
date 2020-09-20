#include "PointLight.h"

PointLight::PointLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
						int lightNumber, glm::vec3 position, float constant, float linear, float quadratic)
{
	this->shaderProgram = shaderProgram;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	if (lightNumber >= MAX_NUMBER_OF_POINT_LIGHTS) lightNumber = MAX_NUMBER_OF_POINT_LIGHTS - 1;
	this->lightNumber = lightNumber;
}

void PointLight::SetLightInShader()
{
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".position").c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".specular").c_str()), specular.x, specular.y, specular.z);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".quadratic").c_str()), quadratic);
}

std::string PointLight::GetNumberedShaderProperty()
{
	std::string number = std::to_string(lightNumber);
	return ("pointLights[" + number + "]").c_str();
}