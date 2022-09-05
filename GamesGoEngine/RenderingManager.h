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
	~RenderingManager();

	static RenderingManager* instance;
	static RenderingManager* GetInstance();

public:
	static void Init(GLint screenWidth, GLint screenHeight);

private:
	void ConfigureFramebuffers(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer);
	static void ConfigureFramebuffer(GLint screenWidth, GLint screenHeight,
		unsigned int& framebuffer, unsigned int& textureColorBuffer,
		unsigned int& depthStencilBuffer, unsigned int& stencilView,
		bool shouldGenerateFramebuffer);

public:
	static void ResizeBuffers(GLint screenWidth, GLint screenHeight);

private:
	void ResizeBuffersInternal(GLint screenWidth, GLint screenHeight);
	void ConfigureUniformBufferObjects();
	void CreateDebugMaterials();

public:
	static void Update();

private:
	void UpdateUniformBufferObjects();
	static void DrawSkybox();
	static void DrawRenderers(const std::vector<MeshRenderer*>& renderers);
	static void DrawRenderers(const std::vector<MeshRenderer*>& renderers, Material* material);
	static void DrawPostProcessEffects();

public:
	static void SetSkybox(SkyboxRenderer* skybox);
	static void AddMeshRenderer(MeshRenderer* meshRenderer);
	static void AddPostProcessRenderer(PostProcessRenderer* postProcessRenderer);
	static void SortMeshRenderers();
	static void SortPostProcessRenderers();

private:
	static void SortOpaqueMeshRenderers();
	static void SortTransparentMeshRenderers();

public:
	static LightsManager* GetLightsManager();
	static void EnablePostProcessing(bool enable);
	static bool IsPostProcessingEnabled();
	static void SetWireframeOnly(bool wireframeOnly);
	static bool IsWireframeOnly();
	static void EnableNormalsDebug(bool enable);
	static bool IsNormalsDebugEnabled();

private:
	static bool CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);
	static bool CompareTransparentRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);
	static bool ComparePostProcessRenderersPositions(PostProcessRenderer* ppr1, PostProcessRenderer* ppr2);

	LightsManager* lightsManager;
	SkyboxRenderer* skybox;
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<MeshRenderer*> opaqueMeshRenderers;
	std::vector<MeshRenderer*> transparentMeshRenderers;
	// container holding only active post process renderers
	std::vector<PostProcessRenderer*> usedPostProcessRenderers;
	// container holding all subscribed post process renderers
	std::vector<PostProcessRenderer*> postProcessRenderers;
	bool postProcessingEnabled = true;
	bool firstRenderedFrame = true;
	bool renderWireframeOnly = false;
	bool normalsDebugEnabled = false;

	// uniform buffers used globally by shaders
	unsigned int uboMatrices;
	unsigned int uboCameraData;
	unsigned int uboTimeData;

	// sets of 2 since we use ping pong rendering
	unsigned int framebuffer1;
	unsigned int framebuffer2;
	
	unsigned int textureColorBuffer1;
	unsigned int textureColorBuffer2;

	unsigned int depthStencilBuffer1;
	unsigned int depthStencilBuffer2;

	unsigned int stencilView1;
	unsigned int stencilView2;

	// debug materials
	Material* normalDebugMaterial;
};