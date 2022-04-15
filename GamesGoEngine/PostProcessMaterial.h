#pragma once

#include "Material.h"

class PostProcessMaterial : public Material
{
public:
	PostProcessMaterial();
	PostProcessMaterial(const GLchar* fragmentPath);

	void SetBlendWeight(float weight) const;

	virtual void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;
};