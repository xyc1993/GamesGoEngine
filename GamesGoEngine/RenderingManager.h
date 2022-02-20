#pragma once

#include <vector>

#include "LightsManager.h"

class MeshRenderer;
class Material;

enum class RenderQueuePosition
{
	Opaque = 1000,
	Transparent = 3000,
	EditorOutline = 4500,
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
	static Material* GetEditorOutlineMaterial(bool isMeshImported);

private:
	static bool CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);

	LightsManager* lightsManager;
	std::vector<MeshRenderer*> meshRenderers;

	// used for outlining selected game objects
	Material* editorOutlineMaterialScale;
	Material* editorOutlineMaterialNormals;
};