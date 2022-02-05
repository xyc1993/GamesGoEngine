#pragma once

#include <vector>

class MeshRenderer;

enum class RenderQueuePosition
{
	Opaque = 1000,
	EditorOutline = 1500,
};

class RenderingManager
{
private:
	RenderingManager() = default;

	static RenderingManager* instance;
	static RenderingManager* GetInstance();

public:
	static void Update();
	static void AddMeshRenderer(MeshRenderer* meshRenderer);
	static void SortMeshRenderers();

private:
	static bool CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);

	std::vector<MeshRenderer*> meshRenderers;
};