#pragma once

#include <map>
#include <tuple>
#include <vector>
#include <glm/fwd.hpp>

#include "Shader.h"
#include "SharedAsset.h"

enum class LightModelType
{
	Unlit,
	LitForward
};

/*
 * Class responsible for handling shaders
 * For now it assumes that it'll be provided with vertex shader & fragment shader (no extra shader types handled now)
 */
class Material : public SharedAsset
{
public:
	Material();
	Material(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Material();

	// all shaders have to support model, view, projection matrices!
	void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void SetShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	
	void SetTexture(const GLchar* textureName, GLuint textureIndex, GLchar* path);
	void SetCubeTexture(const GLchar* textureName, GLuint textureIndex, const std::vector<const GLchar*>& paths);
	
	void SetFloat(const GLchar* floatName, float value) const;
	void SetFloat(const GLint floatID, float value) const;

	void SetLightModel(LightModelType type);
	
private:
	Shader* shader;

	std::map<GLint, std::tuple<GLuint, GLuint>> texturesMap;
	std::map<GLint, std::tuple<GLuint, GLuint>> cubeTexturesMap;
	
	LightModelType lightModelType;	
};