#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

#include "LightsManager.h"
#include "TextureLoader.h"

Material::Material()
{
	shader = nullptr;
	texturesIDs.clear();
	lightModelType = LightModelType::Unlit;
}

Material::Material(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	shader = new Shader(vertexPath, fragmentPath);
	texturesIDs.clear();
	lightModelType = LightModelType::Unlit;
}

void Material::Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	if (shader == nullptr)
	{
		return;
	}

	shader->Use();
	
	for (int i = 0; i < texturesIDs.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + (GLuint)i);
		glBindTexture(GL_TEXTURE_2D, texturesIDs[i]);
	}
	
	const GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	const GLint viewLoc = glGetUniformLocation(shader->Program, "view");
	const GLint projLoc = glGetUniformLocation(shader->Program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	switch (lightModelType)
	{
	case LightModelType::Unlit:
		break;
	case LightModelType::LitForward:
		LightsManager::SetLightsInShader(shader->Program);
		break;
	}
}

void Material::SetShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	delete shader;
	shader = new Shader(vertexPath, fragmentPath);	
}

void Material::SetTexture(GLchar* textureName, GLchar* path)
{
	if (shader != nullptr)
	{
		GLuint texture = TextureLoader::LoadTexture(path);
		texturesIDs.push_back(texture);
	}
}

void Material::SetFloat(const GLchar* floatName, float value) const
{
	const GLint floatID = glGetUniformLocation(shader->Program, floatName);
	SetFloat(floatID, value);
}

void Material::SetFloat(const GLint floatID, float value) const
{
	if (shader != nullptr)
	{
		shader->Use();
		glUniform1f(floatID, value);
	}
}

void Material::SetLightModel(LightModelType type)
{
	lightModelType = type;
}
