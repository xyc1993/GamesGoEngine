#pragma once

#include <map>
#include <tuple>
#include <vector>
#include <glm/fwd.hpp>

#include "Shader.h"

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
	Material(const GLchar* vertexPath, const GLchar* fragmentPath);
	Material(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);
	virtual ~Material();

	// all shaders have to support model, view, projection matrices!
	virtual void Draw(glm::mat4 model);
	void SetShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	
	void SetTextureByPath(const GLchar* textureName, GLuint textureIndex, GLchar* path, bool transparencyEnabled = false, bool sRGB = true);
	void SetTexture(const GLchar* textureName, GLuint textureIndex, GLuint texture);
	GLint GetTexture(const GLchar* textureName) const;

	void SetCubeTextureByPath(const GLchar* textureName, GLuint textureIndex, const std::vector<const GLchar*>& paths, bool transparencyEnabled = false, bool sRGB = true);
	void SetCubeTexture(const GLchar* textureName, GLuint textureIndex, GLuint texture);
	
	void SetFloat(const GLchar* floatName, float value) const;
	void SetFloat(const GLint floatID, float value) const;
	float GetFloat(const GLchar* floatName) const;
	float GetFloat(const GLint floatID) const;

	void SetInt(const GLchar* intName, int value) const;
	void SetInt(const GLint intID, int value) const;

	void SetVector4(const GLchar* vectorName, glm::vec4 value) const;
	void SetVector4(const GLint vectorID, glm::vec4 value) const;

	void SetVector3(const GLchar* vectorName, glm::vec3 value) const;
	void SetVector3(const GLint vectorID, glm::vec3 value) const;

	void SetMat4(const GLchar* matrixName, glm::mat4 value) const;
	void SetMat4(const GLint matrixID, glm::mat4 value) const;

	void SetLightModel(LightModelType type);
	LightModelType GetLightModel() const;

	GLuint GetShaderProgram() const;
	
protected:
	Shader* shader;

	// <textureID <textureIndex, texture>>
	std::map<GLint, std::tuple<GLuint, GLuint>> texturesMap;
	std::map<GLint, std::tuple<GLuint, GLuint>> cubeTexturesMap;
	
	LightModelType lightModelType;	
};