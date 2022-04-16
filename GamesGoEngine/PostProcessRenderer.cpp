#include "PostProcessRenderer.h"

#include "MeshPrimitiveQuad.h"
#include "RenderingManager.h"

PostProcessRenderer::PostProcessRenderer()
{
	mesh = std::make_shared<MeshPrimitiveQuad>();
	materialList.clear();
	materialList.resize(mesh->GetSubMeshesCount());
	RenderingManager::AddPostProcessRenderer(this);
	SetPostProcessOrder(0);
}

void PostProcessRenderer::SetMaterial(const std::shared_ptr<PostProcessMaterial>& material)
{
	if (materialList[0] != material)
	{
		materialList[0].reset();
		materialList[0] = material;
		SetBlendWeight(1.0f);
	}
}

void PostProcessRenderer::SetBlendWeight(float weight) const
{
	if (materialList[0] != nullptr)
	{
		Material* material = materialList[0].get();
		PostProcessMaterial* postProcessMaterial = dynamic_cast<PostProcessMaterial*>(material);
		if (postProcessMaterial != nullptr)
		{
			postProcessMaterial->SetBlendWeight(weight);
		}
	}
}

float PostProcessRenderer::GetBlendWeight() const
{
	if (materialList[0] != nullptr)
	{
		Material* material = materialList[0].get();
		PostProcessMaterial* postProcessMaterial = dynamic_cast<PostProcessMaterial*>(material);
		if (postProcessMaterial != nullptr)
		{
			return postProcessMaterial->GetBlendWeight();
		}
	}

	return -1.0f;
}

void PostProcessRenderer::SetPostProcessOrder(int orderValue)
{
	postProcessOrder = orderValue;
	RenderingManager::SortPostProcessRenderers();
}

int PostProcessRenderer::GetPostProcessOrder() const
{
	return postProcessOrder;
}