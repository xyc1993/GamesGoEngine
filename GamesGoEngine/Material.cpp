#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

#include "RenderingManager.h"
#include "TextureLoader.h"

namespace GamesGoEngine
{
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

	Material::Material(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
	{
		shader = new Shader(vertexPath, fragmentPath, geometryPath);
		texturesMap.clear();
		lightModelType = LightModelType::Unlit;
	}

	Material::~Material()
	{
		delete shader;
	}

	void Material::Draw(glm::mat4 model)
	{
		if (shader == nullptr)
		{
			return;
		}

		for (auto it = texturesMap.begin(); it != texturesMap.end(); ++it)
		{
			glActiveTexture(GL_TEXTURE0 + std::get<0>(it->second));
			glBindTexture(GL_TEXTURE_2D, std::get<1>(it->second));
		}

		for (auto it = cubeTexturesMap.begin(); it != cubeTexturesMap.end(); ++it)
		{
			glActiveTexture(GL_TEXTURE0 + std::get<0>(it->second));
			glBindTexture(GL_TEXTURE_CUBE_MAP, std::get<1>(it->second));
		}

		shader->Use();

		const GLint modelLoc = glGetUniformLocation(shader->GetProgram(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		switch (lightModelType)
		{
		case LightModelType::Unlit:
			break;
		case LightModelType::LitForward:
			break;
		case LightModelType::LitDeferred:
			break;
		}
	}

	void Material::SetShader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		delete shader;
		shader = new Shader(vertexPath, fragmentPath);
	}

	void Material::SetShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
	{
		delete shader;
		shader = new Shader(vertexPath, fragmentPath, geometryPath);
	}

	void Material::SetTextureByPath(const GLchar* textureName, GLuint textureIndex, GLchar* path, bool transparencyEnabled, bool sRGB)
	{
		if (shader != nullptr)
		{
			const GLuint texture = TextureLoader::LoadTexture(path, transparencyEnabled, sRGB);
			SetTexture(textureName, textureIndex, texture);
		}
	}

	void Material::SetTexture(const GLchar* textureName, GLuint textureIndex, GLuint texture)
	{
		if (shader != nullptr)
		{
			const GLint textureID = glGetUniformLocation(shader->GetProgram(), textureName);
			texturesMap[textureID] = std::tuple<GLuint, GLuint>(textureIndex, texture);
			// Tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
			shader->Use();
			glUniform1i(textureID, textureIndex);
		}
	}

	GLint Material::GetTexture(const GLchar* textureName) const
	{
		GLint errValue = 0;
		GLint* texture = &errValue;
		if (shader != nullptr)
		{
			const GLint textureID = glGetUniformLocation(shader->GetProgram(), textureName);
			glGetUniformiv(shader->GetProgram(), textureID, texture);
			return *texture;
		}
		return 0;
	}

	void Material::SetCubeTextureByPath(const GLchar* textureName, GLuint textureIndex, const std::vector<const GLchar*>& paths, bool transparencyEnabled, bool sRGB)
	{
		if (shader != nullptr)
		{
			const GLuint texture = TextureLoader::LoadCubemap(paths, transparencyEnabled, sRGB);
			SetCubeTexture(textureName, textureIndex, texture);
		}
	}

	void Material::SetCubeTexture(const GLchar* textureName, GLuint textureIndex, GLuint texture)
	{
		if (shader != nullptr)
		{
			const GLint textureID = glGetUniformLocation(shader->GetProgram(), textureName);
			cubeTexturesMap[textureID] = std::tuple<GLuint, GLuint>(textureIndex, texture);
			// Tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
			shader->Use();
			glUniform1i(textureID, textureIndex);
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

	float Material::GetFloat(const GLchar* floatName) const
	{
		const GLint floatID = glGetUniformLocation(shader->GetProgram(), floatName);
		return GetFloat(floatID);
	}

	float Material::GetFloat(const GLint floatID) const
	{
		float errValue = -1.0f;
		float* value = &errValue;
		if (shader != nullptr)
		{
			glGetUniformfv(shader->GetProgram(), floatID, value);
		}
		return *value;
	}

	void Material::SetInt(const GLchar* intName, int value) const
	{
		const GLint intID = glGetUniformLocation(shader->GetProgram(), intName);
		SetInt(intID, value);
	}

	void Material::SetInt(const GLint intID, int value) const
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform1i(intID, value);
		}
	}

	void Material::SetVector4(const GLchar* vectorName, glm::vec4 value) const
	{
		const GLint vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		SetVector4(vectorID, value);
	}

	void Material::SetVector4(const GLint vectorID, glm::vec4 value) const
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform4f(vectorID, value.x, value.y, value.z, value.w);
		}
	}

	void Material::SetVector3(const GLchar* vectorName, glm::vec3 value)
	{
		const GLint vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		SetVector3(vectorID, value);
	}

	void Material::SetVector3(const GLint vectorID, glm::vec3 value)
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform3f(vectorID, value.x, value.y, value.z);
			storedVec3Map[vectorID] = value;
		}
	}

	glm::vec3 Material::GetVector3(const GLchar* vectorName) const
	{
		const GLint vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		return GetVector3(vectorID);
	}

	glm::vec3 Material::GetVector3(const GLint vectorID) const
	{
		if (storedVec3Map.find(vectorID) == storedVec3Map.end())
		{
			return glm::vec3(0.0f);
		}
		else
		{
			return storedVec3Map.at(vectorID);
		}
	}

	void Material::SetVector2(const GLchar* vectorName, glm::vec2 value) const
	{
		const GLint vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		SetVector2(vectorID, value);
	}

	void Material::SetVector2(const GLint vectorID, glm::vec2 value) const
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform2f(vectorID, value.x, value.y);
		}
	}

	void Material::SetMat4(const GLchar* matrixName, glm::mat4 value) const
	{
		const GLint matrtixID = glGetUniformLocation(shader->GetProgram(), matrixName);
		SetMat4(matrtixID, value);
	}

	void Material::SetMat4(const GLint matrixID, glm::mat4 value) const
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(value));
		}
	}

	void Material::SetLightModel(LightModelType type)
	{
		lightModelType = type;
	}

	LightModelType Material::GetLightModel() const
	{
		return lightModelType;
	}

	Shader* Material::GetShader() const
	{
		return shader;
	}

	GLuint Material::GetShaderProgram() const
	{
		return shader->GetProgram();
	}

}