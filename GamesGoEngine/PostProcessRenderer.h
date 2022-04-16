#pragma once

#include "PostProcessMaterial.h"
#include "Renderer.h"

class PostProcessRenderer : public Renderer
{
public:
	PostProcessRenderer();

	void SetMaterial(const std::shared_ptr<PostProcessMaterial>& material);
	void SetBlendWeight(float weight) const;
	float GetBlendWeight() const;
	void SetPostProcessOrder(int orderValue);
	int GetPostProcessOrder() const;

private:
	int postProcessOrder;
};