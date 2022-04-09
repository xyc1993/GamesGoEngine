#pragma once

#include "Renderer.h"

class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer();
	
	virtual void Draw() override;
	void SetMaterial(const std::shared_ptr<Material>& material);

protected:
	virtual glm::mat4 GetViewMatrix() const override;
};