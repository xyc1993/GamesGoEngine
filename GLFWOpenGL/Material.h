#pragma once

#include <glm/fwd.hpp>

#include "Shader.h"

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

private:
	Shader* shader;
};
