#include "SpotLight.h"

SpotLight::SpotLight()
{

}

SpotLight::SpotLight(GLuint lightNumber, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
	float cutOff, float outerCutOff)
{
	this->lightNumber = lightNumber;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->position = position;
	this->direction = direction;

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
}

void SpotLight::SetLightInShader(const GLuint& shaderProgram)
{
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".specular").c_str()), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".position").c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".direction").c_str()), direction.x, direction.y, direction.z);

	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".quadratic").c_str()), quadratic);

	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".cutOff").c_str()), cutOff);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".outerCutOff").c_str()), outerCutOff);
}

void SpotLight::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void SpotLight::SetPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void SpotLight::SetDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void SpotLight::SetDirection(float x, float y, float z)
{
	this->direction.x = x;
	this->direction.y = y;
	this->direction.z = z;
}

std::string SpotLight::GetNumberedShaderProperty()
{
	std::string number = std::to_string(lightNumber);
	return ("spotLight[" + number + "]").c_str();
}