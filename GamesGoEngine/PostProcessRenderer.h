#pragma once

#include "PostProcessMaterial.h"
#include "Renderer.h"

/*
 * Post process renderer handles holding post process material list & possibly passing transform data to materials in case of local post process volumes.
 */
class PostProcessRenderer : public Renderer
{
public:
	PostProcessRenderer();
	
	virtual void Draw() override;
	void AddMaterial(const std::shared_ptr<PostProcessMaterial>& material);
	void RemoveMaterial(size_t materialIndex);
	void SetBlendWeight(float weight, size_t materialIndex) const;
	float GetBlendWeight(size_t materialIndex) const;

private:
	std::vector<std::shared_ptr<PostProcessMaterial>> ppMaterialList;
};