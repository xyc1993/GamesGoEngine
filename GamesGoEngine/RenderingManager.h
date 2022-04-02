#pragma once

#include <vector>

#include "LightsManager.h"
#include "PostProcessMaterial.h"
#include "Skybox.h"

class PostProcessRenderer;
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
	static void Init(GLint screenWidth, GLint screenHeight);
	static void ResizeBuffers(GLint screenWidth, GLint screenHeight);
	static void Update();

private:
	static void DrawSkybox();
	static void DrawRenderers(const std::vector<MeshRenderer*>& renderers);
	static void DrawPostProcessEffects();

public:
	static void SetSkybox(Skybox* skybox);
	static void AddMeshRenderer(MeshRenderer* meshRenderer);
	static void AddPostProcessRenderer(PostProcessRenderer* postProcessRenderer);
	static void SortMeshRenderers();

private:
	static void SortOpaqueMeshRenderers();
	static void SortTransparentMeshRenderers();

public:
	static LightsManager* GetLightsManager();
	static Material* GetEditorOutlineMaterial(bool isMeshImported);

private:
	static bool CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);
	static bool CompareTransparentRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);

	LightsManager* lightsManager;
	Skybox* skybox;
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<MeshRenderer*> opaqueMeshRenderers;
	std::vector<MeshRenderer*> transparentMeshRenderers;
	std::vector<PostProcessRenderer*> postProcessRenderers;

	// used for outlining selected game objects
	Material* editorOutlineMaterialScale;
	Material* editorOutlineMaterialNormals;

	static unsigned int framebuffer;
	static unsigned int framebuffer2;
	
	static unsigned int textureColorbuffer;
	static unsigned int textureColorbuffer2;

	static unsigned int rbo;
	static unsigned int rbo2;
};