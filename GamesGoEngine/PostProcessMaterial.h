#pragma once

#include "Material.h"

class PostProcessMaterial : public Material
{
public:
	PostProcessMaterial();
	PostProcessMaterial(const GLchar* fragmentPath);

	void SetBlendWeight(float weight) const;
	float GetBlendWeight() const;

	virtual void Draw(glm::mat4 model, glm::vec3 cameraPos) override;
};