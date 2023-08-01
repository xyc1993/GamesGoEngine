#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "Component.h"

class Light : public Component
{
public:
	/*
	 * Method used to set light in shader
	 * Arguments:
	 * shaderProgram - shader that will receive light data
	 * isNumberedLight - should light be parsed as an element of the array in the shader
	 * overrideThisLightNumber - should light index in shader array be overriden
	 * thisLightNumberOverride - array index override
	 * overrideLightsNumber - should light array size in shader be overriden
	 * lightsNumberOverride - array size override
	 */
	virtual void SetLightInShader(const GLuint& shaderProgram, bool isNumberedLight = true,
		bool overrideThisLightNumber = false, int thisLightNumberOverride = 0,
		bool overrideLightsNumber = false, int lightsNumberOverride = 0) = 0;

	static constexpr GLuint INITIALIZATION_ERROR = -1;

protected:
	virtual std::string GetNumberedShaderProperty(int lightNumber) = 0;

	// light number (index) in shader array
	GLuint lightNumber;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};