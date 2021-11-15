#pragma once

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

	// all shaders have to support model, view, projection matrices!
	void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void SetShader(const GLchar* vertexPath, const GLchar* fragmentPath);

	//TO DO: should have some safety measure against adding texture that was already added
	void AddTexture(GLchar* path, GLchar* textureName);
	//TO DO: should have some safety measure against adding float that was already added
	void AddFloat(float value, GLchar* floatName);

	void SetLightModel(LightModelType type);

private:
	Shader* shader;

	std::vector<GLuint> texturesIDs;
	std::vector<GLuint> floatIDs;
	std::vector<float> floatValues;
	LightModelType lightModelType;
};