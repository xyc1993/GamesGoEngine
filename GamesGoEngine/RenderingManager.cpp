#include "RenderingManager.h"

#include <algorithm>

#include "MeshRenderer.h"

RenderingManager* RenderingManager::instance = new RenderingManager();

RenderingManager* RenderingManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new RenderingManager();
	}
	return instance;
}

void RenderingManager::Update()
{
	for (size_t i = 0; i < GetInstance()->meshRenderers.size(); i++)
	{
		MeshRenderer* meshRenderer = GetInstance()->meshRenderers[i];
		if (meshRenderer != nullptr)
		{
			meshRenderer->Draw();
		}
	}
}

void RenderingManager::AddMeshRenderer(MeshRenderer* meshRenderer)
{
	GetInstance()->meshRenderers.push_back(meshRenderer);
	SortMeshRenderers();
}

void RenderingManager::SortMeshRenderers()
{
	std::sort(GetInstance()->meshRenderers.begin(), GetInstance()->meshRenderers.end(), CompareRenderersPositions);
}

bool RenderingManager::CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2)
{
	return (mr1->GetRenderQueuePosition() < mr2->GetRenderQueuePosition());
}