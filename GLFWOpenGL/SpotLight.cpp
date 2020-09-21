#include "SpotLight.h"

SpotLight::SpotLight()
{

}

SpotLight::SpotLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
	float cutOff, float outerCutOff)
{
	this->shaderProgram = shaderProgram;
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

void SpotLight::SetLightInShader()
{
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.direction"), direction.x, direction.y, direction.z);
	
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.constant"), constant);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.linear"), linear);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.quadratic"), quadratic);
	
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutOff"), cutOff);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutOff"), outerCutOff);
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