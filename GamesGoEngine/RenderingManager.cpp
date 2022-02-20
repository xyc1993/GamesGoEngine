#include "RenderingManager.h"

#include <algorithm>

#include "CamerasManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"

RenderingManager* RenderingManager::instance = nullptr;

RenderingManager::RenderingManager()
{
	lightsManager = new LightsManager();
	
	// set the outline materials
	editorOutlineMaterialScale = new Material("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");
	editorOutlineMaterialNormals = new Material("res/shaders/unlitProtruded.vert.glsl", "res/shaders/unlit.frag.glsl");
	
	constexpr glm::vec3 editorOutlineColor = glm::vec3(1.0f, 0.8f, 0.0f);
	editorOutlineMaterialScale->SetVector3((GLchar*)"unlitColor", editorOutlineColor);
	editorOutlineMaterialNormals->SetVector3((GLchar*)"unlitColor", editorOutlineColor);

	constexpr float editorOutlineNormalsProtrusion = 0.03f;
	editorOutlineMaterialNormals->SetFloat((GLchar*)"protrusion", editorOutlineNormalsProtrusion);
}

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
	// TODO:: more optimal sorting, it could sort on camera view change, not on every draw frame
	SortTransparentMeshRenderers();

	DrawRenderers(GetInstance()->opaqueMeshRenderers);
	DrawRenderers(GetInstance()->transparentMeshRenderers);
}

void RenderingManager::DrawRenderers(const std::vector<MeshRenderer*>& renderers)
{
	for (size_t i = 0; i < renderers.size(); i++)
	{
		MeshRenderer* meshRenderer = renderers[i];
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
	GetInstance()->opaqueMeshRenderers.clear();
	GetInstance()->transparentMeshRenderers.clear();

	for (size_t i = 0; i < GetInstance()->meshRenderers.size(); i++)
	{
		MeshRenderer* meshRenderer = GetInstance()->meshRenderers[i];
		if (meshRenderer->GetRenderQueuePosition() < static_cast<int>(RenderQueuePosition::Transparent))
		{
			GetInstance()->opaqueMeshRenderers.push_back(meshRenderer);
		}
		else
		{
			GetInstance()->transparentMeshRenderers.push_back(meshRenderer);
		}
	}
	
	SortOpaqueMeshRenderers();
	SortTransparentMeshRenderers();
}

void RenderingManager::SortOpaqueMeshRenderers()
{
	std::sort(GetInstance()->opaqueMeshRenderers.begin(), GetInstance()->opaqueMeshRenderers.end(), CompareRenderersPositions);
}

void RenderingManager::SortTransparentMeshRenderers()
{
	// For now we assume 2 things:
	// 1) The order in render queue doesn't matter if it's after transparent
	// 2) For now we use simple & naive sort based on distance from the camera
	std::sort(GetInstance()->transparentMeshRenderers.begin(), GetInstance()->transparentMeshRenderers.end(), CompareTransparentRenderersPositions);
}

LightsManager* RenderingManager::GetLightsManager()
{
	return GetInstance()->lightsManager;
}

Material* RenderingManager::GetEditorOutlineMaterial(bool isMeshImported)
{
	if (isMeshImported)
	{
		return GetInstance()->editorOutlineMaterialNormals;
	}
	else
	{
		return GetInstance()->editorOutlineMaterialScale;
	}
}

bool RenderingManager::CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2)
{
	return (mr1->GetRenderQueuePosition() < mr2->GetRenderQueuePosition());
}

bool RenderingManager::CompareTransparentRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2)
{
	if (mr1->GetOwner() != nullptr && mr2->GetOwner() != nullptr)
	{
		const float length1 = glm::length(mr1->GetOwner()->GetTransform()->GetPosition() - CamerasManager::GetActiveCameraPosition());
		const float length2 = glm::length(mr2->GetOwner()->GetTransform()->GetPosition() - CamerasManager::GetActiveCameraPosition());
		return (length1 > length2);
	}
	else
	{
		return CompareRenderersPositions(mr1, mr2);
	}
}