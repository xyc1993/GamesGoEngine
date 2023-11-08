#pragma once

#include "Light.h"

namespace GamesGoEngine
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
		~DirectionalLight();

		void Update() override;
		void SetLightInShader(const unsigned int& shaderProgram, bool isNumberedLight = true,
			bool overrideThisLightNumber = false, int thisLightNumberOverride = 0,
			bool overrideLightsNumber = false, int lightsNumberOverride = 0) override;

	protected:
		std::string GetNumberedShaderProperty(int lightNumber) override;
		virtual void InitMetaData() override;

	private:
		glm::vec3 direction;
	};
}