#pragma once

#include <vector>

#include "LightsManager.h"

class MeshRenderer;

enum class RenderQueuePosition
{
	Opaque = 1000,
	EditorOutline = 1500,
};

class RenderingManager
{
private:
	RenderingManager();

	static RenderingManager* instance;
	static RenderingManager* GetInstance();

public:
	static void Update();
	static void AddMeshRenderer(MeshRenderer* meshRenderer);
	static void SortMeshRenderers();
	static LightsManager* GetLightsManager();

private:
	static bool CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);

	LightsManager* lightsManager;
	std::vector<MeshRenderer*> meshRenderers;
};