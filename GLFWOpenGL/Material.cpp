#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

Material::Material()
{
	shader = nullptr;
}

Material::Material(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	shader = new Shader(vertexPath, fragmentPath);
}

void Material::Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	const GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	const GLint viewLoc = glGetUniformLocation(shader->Program, "view");
	const GLint projLoc = glGetUniformLocation(shader->Program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Material::SetShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	if (shader != nullptr)
	{
		shader->~Shader();
	}
	shader = new Shader(vertexPath, fragmentPath);	
}
