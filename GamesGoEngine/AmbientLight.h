#pragma once

#include "Light.h"

class AmbientLight : public Light
{
public:
	AmbientLight();
	AmbientLight(glm::vec3 ambientColor);
	~AmbientLight();

	void CopyData(const AmbientLight& other);
	void SetThisLightInShader(const GLuint& shaderProgram) override;
	void SetLightInShader(const GLuint& shaderProgram) override;

protected:
	virtual std::string GetNumberedShaderProperty(int lightNumber) override;
};