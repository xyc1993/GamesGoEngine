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

	Material::Material(const char* vertexPath, const char* fragmentPath)
	{
		shader = new Shader(vertexPath, fragmentPath);
		texturesMap.clear();
		lightModelType = LightModelType::Unlit;
	}

	Material::Material(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
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

		const int modelLoc = glGetUniformLocation(shader->GetProgram(), "model");
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

	void Material::SetShader(const char* vertexPath, const char* fragmentPath)
	{
		delete shader;
		shader = new Shader(vertexPath, fragmentPath);
		CleanUniformsData();
	}

	void Material::SetShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
	{
		delete shader;
		shader = new Shader(vertexPath, fragmentPath, geometryPath);
		CleanUniformsData();
	}

	void Material::CleanUniformsData()
	{
		texturesMap.clear();
		cubeTexturesMap.clear();

		texturesPathsMap.clear();

		storedIntegersMap.clear();
		storedFloatsMap.clear();
		storedVec2Map.clear();
		storedVec3Map.clear();
		storedVec4Map.clear();
	}

	void Material::SetTextureByPath(const char* textureName, unsigned int textureIndex, const char* path, bool transparencyEnabled, bool sRGB)
	{
		if (shader != nullptr)
		{
			const unsigned int texture = TextureLoader::LoadTexture(path, transparencyEnabled, sRGB);
			SetTexture(textureName, textureIndex, texture);
			std::string texName = textureName;
			std::string texPath = path;
			texturesPathsMap[textureName] = texPath;
		}
	}

	void Material::SetTexture(const char* textureName, unsigned int textureIndex, unsigned int texture)
	{
		if (shader != nullptr)
		{
			const int textureID = glGetUniformLocation(shader->GetProgram(), textureName);
			texturesMap[textureID] = std::tuple<unsigned int, unsigned int>(textureIndex, texture);
			// Tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
			shader->Use();
			glUniform1i(textureID, textureIndex);
		}
	}

	int Material::GetTexture(const char* textureName) const
	{
		int errValue = 0;
		int* texture = &errValue;
		if (shader != nullptr)
		{
			const int textureID = glGetUniformLocation(shader->GetProgram(), textureName);
			glGetUniformiv(shader->GetProgram(), textureID, texture);
			return *texture;
		}
		return 0;
	}

	std::string Material::GetTexturePath(const std::string& textureName) const
	{
		if (texturesPathsMap.find(textureName) == texturesPathsMap.end())
		{
			return "";
		}
		else
		{
			return texturesPathsMap.at(textureName);
		}
	}

	void Material::SetCubeTextureByPath(const char* textureName, unsigned int textureIndex, const std::vector<const char*>& paths, bool transparencyEnabled, bool sRGB)
	{
		if (shader != nullptr)
		{
			const unsigned int texture = TextureLoader::LoadCubemap(paths, transparencyEnabled, sRGB);
			SetCubeTexture(textureName, textureIndex, texture);
		}
	}

	void Material::SetCubeTexture(const char* textureName, unsigned int textureIndex, unsigned int texture)
	{
		if (shader != nullptr)
		{
			const int textureID = glGetUniformLocation(shader->GetProgram(), textureName);
			cubeTexturesMap[textureID] = std::tuple<unsigned int, unsigned int>(textureIndex, texture);
			// Tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
			shader->Use();
			glUniform1i(textureID, textureIndex);
		}
	}

	void Material::SetInt(const char* intName, int value)
	{
		const int intID = glGetUniformLocation(shader->GetProgram(), intName);
		SetInt(intID, value);
	}

	void Material::SetInt(const int intID, int value)
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform1i(intID, value);
			storedIntegersMap[intID] = value;
		}
	}

	int Material::GetInt(const char* intName) const
	{
		const int intID = glGetUniformLocation(shader->GetProgram(), intName);
		return GetFloat(intID);
	}

	int Material::GetInt(const int intID) const
	{
		if (storedIntegersMap.find(intID) == storedIntegersMap.end())
		{
			return 0;
		}
		else
		{
			return storedIntegersMap.at(intID);
		}
	}

	void Material::SetFloat(const char* floatName, float value)
	{
		const int floatID = glGetUniformLocation(shader->GetProgram(), floatName);
		SetFloat(floatID, value);
	}

	void Material::SetFloat(const int floatID, float value)
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform1f(floatID, value);
			storedFloatsMap[floatID] = value;
		}
	}

	float Material::GetFloat(const char* floatName) const
	{
		const int floatID = glGetUniformLocation(shader->GetProgram(), floatName);
		return GetFloat(floatID);
	}

	float Material::GetFloat(const int floatID) const
	{
		if (storedFloatsMap.find(floatID) == storedFloatsMap.end())
		{
			return 0.0f;
		}
		else
		{
			return storedFloatsMap.at(floatID);
		}
	}

	void Material::SetVector2(const char* vectorName, glm::vec2 value)
	{
		const int vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		SetVector2(vectorID, value);
	}

	void Material::SetVector2(const int vectorID, glm::vec2 value)
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform2f(vectorID, value.x, value.y);
			storedVec2Map[vectorID] = value;
		}
	}

	glm::vec2 Material::GetVector2(const char* vectorName) const
	{
		const int vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		return GetVector2(vectorID);
	}

	glm::vec2 Material::GetVector2(const int vectorID) const
	{
		if (storedVec2Map.find(vectorID) == storedVec2Map.end())
		{
			return glm::vec2(0.0f);
		}
		else
		{
			return storedVec2Map.at(vectorID);
		}
	}

	void Material::SetVector3(const char* vectorName, glm::vec3 value)
	{
		const int vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		SetVector3(vectorID, value);
	}

	void Material::SetVector3(const int vectorID, glm::vec3 value)
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform3f(vectorID, value.x, value.y, value.z);
			storedVec3Map[vectorID] = value;
		}
	}

	glm::vec3 Material::GetVector3(const char* vectorName) const
	{
		const int vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		return GetVector3(vectorID);
	}

	glm::vec3 Material::GetVector3(const int vectorID) const
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

	void Material::SetVector4(const char* vectorName, glm::vec4 value)
	{
		const int vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		SetVector4(vectorID, value);
	}

	void Material::SetVector4(const int vectorID, glm::vec4 value)
	{
		if (shader != nullptr)
		{
			shader->Use();
			glUniform4f(vectorID, value.x, value.y, value.z, value.w);
			storedVec4Map[vectorID] = value;
		}
	}

	glm::vec4 Material::GetVector4(const char* vectorName) const
	{
		const int vectorID = glGetUniformLocation(shader->GetProgram(), vectorName);
		return GetVector4(vectorID);
	}

	glm::vec4 Material::GetVector4(const int vectorID) const
	{
		if (storedVec4Map.find(vectorID) == storedVec4Map.end())
		{
			return glm::vec4(0.0f);
		}
		else
		{
			return storedVec4Map.at(vectorID);
		}
	}

	void Material::SetMat4(const char* matrixName, glm::mat4 value) const
	{
		const int matrtixID = glGetUniformLocation(shader->GetProgram(), matrixName);
		SetMat4(matrtixID, value);
	}

	void Material::SetMat4(const int matrixID, glm::mat4 value) const
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

	unsigned int Material::GetShaderProgram() const
	{
		return shader->GetProgram();
	}

}