#pragma once

#include "Light.h"

namespace GamesGoEngine
{
	class AmbientLight : public Light
	{
	public:
		AmbientLight();
		AmbientLight(glm::vec3 ambientColor);
		~AmbientLight();

		void CopyData(const AmbientLight& other);
		void Update() override;
		void SetLightInShader(const GLuint& shaderProgram, bool isNumberedLight = true,
			bool overrideThisLightNumber = false, int thisLightNumberOverride = 0,
			bool overrideLightsNumber = false, int lightsNumberOverride = 0) override;

	protected:
		std::string GetNumberedShaderProperty(int lightNumber) override;		
		virtual void InitMetaData() override;
	};
}