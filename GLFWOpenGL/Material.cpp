#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

#include "LightsManager.h"
#include "TextureLoader.h"

Material::Material()
{
	shader = nullptr;
	texturesMap.clear();
	lightModelType = LightModelType::Unlit;
}

Material::Material(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	shader = new Shader(vertexPath, fragmentPath);
	texturesMap.clear();
	lightModelType = LightModelType::Unlit;
}

Material::~Material()
{
	delete shader;
}

void Material::Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	if (shader == nullptr)
	{
		return;
	}

	shader->Use();
	
	for (auto it = texturesMap.begin(); it != texturesMap.end(); ++it)
	{
		glActiveTexture(GL_TEXTURE0 + std::get<0>(it->second));
		glBindTexture(GL_TEXTURE_2D, std::get<1>(it->second));
	}
	
	const GLint modelLoc = glGetUniformLocation(shader->GetProgram(), "model");
	const GLint viewLoc = glGetUniformLocation(shader->GetProgram(), "view");
	const GLint projLoc = glGetUniformLocation(shader->GetProgram(), "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	switch (lightModelType)
	{
	case LightModelType::Unlit:
		break;
	case LightModelType::LitForward:
		LightsManager::SetLightsInShader(shader->GetProgram());
		break;
	}
}

void Material::SetShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	delete shader;
	shader = new Shader(vertexPath, fragmentPath);	
}

void Material::SetTexture(const GLchar* textureName, GLuint textureIndex, GLchar* path)
{
	if (shader != nullptr)
	{
		const GLint textureID = glGetUniformLocation(shader->GetProgram(), textureName);
		GLuint texture = TextureLoader::LoadTexture(path);
		texturesMap[textureID] = std::tuple<GLuint, GLuint>(textureIndex, texture);
	}
}

void Material::SetFloat(const GLchar* floatName, float value) const
{
	const GLint floatID = glGetUniformLocation(shader->GetProgram(), floatName);
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