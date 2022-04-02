#pragma once

#include "PostProcessMaterial.h"
#include "Renderer.h"

class PostProcessRenderer : public Renderer
{
public:
	PostProcessRenderer();

	void SetMaterial(const std::shared_ptr<PostProcessMaterial>& material);
};