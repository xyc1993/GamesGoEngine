#include "PostProcessRenderer.h"

#include "MeshPrimitivesPool.h"
#include "RenderingManager.h"

namespace GamesGoEngine
{
	REGISTER_CLASS_STATIC(PostProcessRenderer);

	PostProcessRenderer::PostProcessRenderer()
	{
		ppMaterialList.clear();
	}

	void PostProcessRenderer::Draw()
	{
		// Drawing post processes is handled by Rendering manager, so we want to leave this method empty. 
	}

	void PostProcessRenderer::AddMaterial(const std::shared_ptr<PostProcessMaterial>& material)
	{
		material->SetBlendWeight(1.0f);
		ppMaterialList.push_back(material);
		RenderingManager::AddPostProcessMaterial(material);
	}

	void PostProcessRenderer::RemoveMaterial(size_t materialIndex)
	{
		RenderingManager::RemovePostProcessMaterial(ppMaterialList[materialIndex]);
		ppMaterialList.erase(ppMaterialList.begin() + materialIndex);
	}

	void PostProcessRenderer::SetBlendWeight(float weight, size_t materialIndex) const
	{
		if (materialIndex < ppMaterialList.size())
		{
			Material* material = ppMaterialList[materialIndex].get();
			PostProcessMaterial* postProcessMaterial = dynamic_cast<PostProcessMaterial*>(material);
			if (postProcessMaterial != nullptr)
			{
				postProcessMaterial->SetBlendWeight(weight);
			}
		}
	}

	float PostProcessRenderer::GetBlendWeight(size_t materialIndex) const
	{
		if (materialIndex < ppMaterialList.size())
		{
			Material* material = ppMaterialList[materialIndex].get();
			const PostProcessMaterial* postProcessMaterial = dynamic_cast<PostProcessMaterial*>(material);
			if (postProcessMaterial != nullptr)
			{
				return postProcessMaterial->GetBlendWeight();
			}
		}
		return -1.0f;
	}

	void PostProcessRenderer::InitMetaData()
	{
		metaData.className = CLASS_NAME(PostProcessRenderer);
	}
}