#pragma once

#include <map>
#include <tuple>
#include <vector>
#include <glm/fwd.hpp>

#include "Shader.h"

enum class LightModelType
{
	Unlit,
	LitForward
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
	virtual ~Material();

	// all shaders have to support model, view, projection matrices!
	virtual void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void SetShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	
	void SetTextureByPath(const GLchar* textureName, GLuint textureIndex, GLchar* path, bool transparencyEnabled = false);
	void SetTexture(const GLchar* textureName, GLuint textureIndex, GLuint texture);
	void SetCubeTextureByPath(const GLchar* textureName, GLuint textureIndex, const std::vector<const GLchar*>& paths);
	
	void SetFloat(const GLchar* floatName, float value) const;
	void SetFloat(const GLint floatID, float value) const;

	void SetVector4(const GLchar* vectorName, glm::vec4 value) const;
	void SetVector4(const GLint vectorID, glm::vec4 value) const;

	void SetVector3(const GLchar* vectorName, glm::vec3 value) const;
	void SetVector3(const GLint vectorID, glm::vec3 value) const;

	void SetLightModel(LightModelType type);
	
protected:
	Shader* shader;

	// <textureID <textureIndex, texture>>
	std::map<GLint, std::tuple<GLuint, GLuint>> texturesMap;
	std::map<GLint, std::tuple<GLuint, GLuint>> cubeTexturesMap;
	
	LightModelType lightModelType;	
};