#include "RenderingManager.h"

#include <algorithm>

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