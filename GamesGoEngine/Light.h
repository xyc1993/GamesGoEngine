#pragma once

#include <glm/glm.hpp>
#include <string>

#include "Component.h"

namespace GamesGoEngine
{
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
		virtual void SetLightInShader(const unsigned int& shaderProgram, bool isNumberedLight = true,
			bool overrideThisLightNumber = false, int thisLightNumberOverride = 0,
			bool overrideLightsNumber = false, int lightsNumberOverride = 0) = 0;

		static constexpr unsigned int INITIALIZATION_ERROR = -1;

	protected:
		virtual std::string GetNumberedShaderProperty(int lightNumber) = 0;

		// light number (index) in shader array
		unsigned int lightNumber;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
}