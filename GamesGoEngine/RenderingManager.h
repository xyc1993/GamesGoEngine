#pragma once

#include <vector>

#include "LightsManager.h"
#include "PostProcessMaterial.h"
#include "SkyboxRenderer.h"

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
	static void SetSkybox(SkyboxRenderer* skybox);
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
	SkyboxRenderer* skybox;
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<MeshRenderer*> opaqueMeshRenderers;
	std::vector<MeshRenderer*> transparentMeshRenderers;
	std::vector<PostProcessRenderer*> postProcessRenderers;

	// used for outlining selected game objects
	Material* editorOutlineMaterialScale;
	Material* editorOutlineMaterialNormals;

	// sets of 2 since we use ping pong rendering
	static unsigned int framebuffer1;
	static unsigned int framebuffer2;
	
	static unsigned int textureColorBuffer1;
	static unsigned int textureColorBuffer2;

	static unsigned int depthStencilBuffer1;
	static unsigned int depthStencilBuffer2;
};