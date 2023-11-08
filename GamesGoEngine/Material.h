#pragma once

#include <map>
#include <tuple>
#include <vector>
#include <glm/fwd.hpp>

#include "Shader.h"

namespace GamesGoEngine
{
	enum class LightModelType
	{
		Unlit,
		LitForward,
		LitDeferred
	};

	/*
	 * Class responsible for handling shaders
	 * For now it assumes that it'll be provided with vertex shader & fragment shader (no extra shader types handled now)
	 */
	class Material
	{
	public:
		Material();
		Material(const char* vertexPath, const char* fragmentPath);
		Material(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
		virtual ~Material();

		// all shaders have to support model, view, projection matrices!
		virtual void Draw(glm::mat4 model);
		void SetShader(const char* vertexPath, const char* fragmentPath);
		void SetShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
		void CleanUniformsData();

		void SetTextureByPath(const char* textureName, unsigned int textureIndex, char* path, bool transparencyEnabled = false, bool sRGB = true);
		void SetTexture(const char* textureName, unsigned int textureIndex, unsigned int texture);
		int GetTexture(const char* textureName) const;

		void SetCubeTextureByPath(const char* textureName, unsigned int textureIndex, const std::vector<const char*>& paths, bool transparencyEnabled = false, bool sRGB = true);
		void SetCubeTexture(const char* textureName, unsigned int textureIndex, unsigned int texture);

		void SetFloat(const char* floatName, float value) const;
		void SetFloat(const int floatID, float value) const;
		float GetFloat(const char* floatName) const;
		float GetFloat(const int floatID) const;

		void SetInt(const char* intName, int value) const;
		void SetInt(const int intID, int value) const;

		void SetVector4(const char* vectorName, glm::vec4 value) const;
		void SetVector4(const int vectorID, glm::vec4 value) const;

		void SetVector3(const char* vectorName, glm::vec3 value);
		void SetVector3(const int vectorID, glm::vec3 value);
		glm::vec3 GetVector3(const char* vectorName) const;
		glm::vec3 GetVector3(const int vectorID) const;

		void SetVector2(const char* vectorName, glm::vec2 value) const;
		void SetVector2(const int vectorID, glm::vec2 value) const;

		void SetMat4(const char* matrixName, glm::mat4 value) const;
		void SetMat4(const int matrixID, glm::mat4 value) const;

		void SetLightModel(LightModelType type);
		LightModelType GetLightModel() const;

		Shader* GetShader() const;
		unsigned int GetShaderProgram() const;

	protected:
		Shader* shader;

		// <textureID <textureIndex, texture>>
		std::map<int, std::tuple<unsigned int, unsigned int>> texturesMap;
		std::map<int, std::tuple<unsigned int, unsigned int>> cubeTexturesMap;

		// Stored uniform variables
		std::map<int, glm::vec3> storedVec3Map;

		LightModelType lightModelType;
	};
}