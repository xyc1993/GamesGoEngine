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
	void InitGammaCorrection();
	void ConfigureFramebuffers(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer);
	static void ConfigureFramebuffer(GLint screenWidth, GLint screenHeight,
		unsigned int& framebuffer, unsigned int& textureColorBuffer,
		unsigned int& depthStencilBuffer, unsigned int& stencilView,
		bool shouldGenerateFramebuffer);
	static void ConfigureShadowMapFramebuffer(GLint shadowMapWidth, GLint shadowMapHeight,
		unsigned int& framebuffer, unsigned int& shadowMap,
		bool shouldGenerateFramebuffer);
	static void ConfigureShadowCubeMapFramebuffer(GLint shadowMapWidth, GLint shadowMapHeight,
		unsigned int& framebuffer, unsigned int& shadowCubeMap,
		bool shouldGenerateFramebuffer);
	void ConfigureGBuffer(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer);

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
	// Updates data such as positions, normals, albedo, specular
	void UpdateGBuffer();
	void UpdateDeferredShading();
	void UpdateDirectionalShadowMap(Light* directionalLight);
	void UpdateOmnidirectionalShadowMap(Light* pointLight);
	void DrawOrientationDebug() const;
	static void DrawSkybox();
	static void DrawRenderers(const std::vector<MeshRenderer*>& renderers);
	static void DrawRenderersOfLightModel(const std::vector<MeshRenderer*>& renderers, LightModelType lightModel);
	static void DrawRenderersExceptLightModel(const std::vector<MeshRenderer*>& renderers, LightModelType lightModel);
	static void DrawRenderers(const std::vector<MeshRenderer*>& renderers, Material* material);
	static void DrawShadowCastingRenderers(const std::vector<MeshRenderer*>& renderers, Material* material);
	static void DrawPostProcessEffects();
	static void ApplyLightForRenderers(Light* light, const std::vector<MeshRenderer*>& renderers);
	static void ClearLightsForRenderers(const std::vector<MeshRenderer*>& renderers);

public:
	static void SetSkybox(SkyboxRenderer* skybox);
	static void AddMeshRenderer(MeshRenderer* meshRenderer);
	static void RemoveMeshRenderer(MeshRenderer* meshRenderer);
	static void AddPostProcessMaterial(const std::shared_ptr<PostProcessMaterial>& ppMaterial);
	static void RemovePostProcessMaterial(const std::shared_ptr<PostProcessMaterial>& ppMaterial);
	static void SortMeshRenderers();
	static void SortPostProcessMaterials();

private:
	static void SortOpaqueMeshRenderers();
	static void SortTransparentMeshRenderers();

public:
	static LightsManager* GetLightsManager();
	static void SetSelectedGameObject(GameObject* selectedObject);
	static void EnablePostProcessing(bool enable);
	static bool IsPostProcessingEnabled();
	static void EnableBloom(bool enable);
	static bool IsBloomEnabled();
	static void EnableHDRToneMappingAndGamma(bool enable);
	static bool IsHDRToneMappingAndGammaEnabled();
	static void SetWireframeOnly(bool wireframeOnly);
	static bool IsWireframeOnly();
	static void EnableNormalsDebug(bool enable);
	static bool IsNormalsDebugEnabled();
	static void SetGamma(float gammaVal);
	static float GetGamma();
private:
	void SetGammaInternal(float gammaVal);
public:
	static void SetExposure(float exposureVal);
	static float GetExposure();
private:
	void SetExposureInternal(float exposureVal);
public:
	static void SetBloomBlurAmount(int amount);
	static int GetBloomBlurAmount();
	// assume square textures
	static void SetShadowMapResolution(unsigned int shadowMapRes);
	static unsigned int GetShadowMapResolution();
private:
	void SetShadowMapResolutionInternal(unsigned int shadowMapRes);
	
private:
	static bool CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);
	static bool CompareTransparentRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);
	static bool ComparePostProcessMaterialsPositions(const std::shared_ptr<PostProcessMaterial>& ppm1, const std::shared_ptr<PostProcessMaterial>& ppm2);

	LightsManager* lightsManager;
	SkyboxRenderer* skybox;
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<MeshRenderer*> opaqueMeshRenderers;
	std::vector<MeshRenderer*> transparentMeshRenderers;
	GameObject* selectedGameObject;
	// container holding only active post process renderers
	std::vector<std::shared_ptr<PostProcessMaterial>> usedPostProcessMaterials;
	// container holding all subscribed post process renderers
	std::vector<std::shared_ptr<PostProcessMaterial>> postProcessMaterials;
	bool postProcessingEnabled = true;
	bool bloomEnabled = true;
	bool hdrTonemappingAndGamma = true;
	bool firstRenderedFrame = true;
	bool renderWireframeOnly = false;
	bool normalsDebugEnabled = false;
	float gamma = 2.2f;
	float exposure = 1.0f;
	int bloomBlurAmount = 10;
	unsigned int shadowWidth = 1024;
	unsigned int shadowHeight = 1024;

	// uniform buffers used globally by shaders
	unsigned int uboMatrices;
	unsigned int uboCameraData;
	unsigned int uboTimeData;

	// sets of 2 since we use ping pong rendering
	unsigned int framebuffer1;
	unsigned int framebuffer2;
	unsigned int shadowFBO1;
	unsigned int shadowFBO2;
	unsigned int bloomFBO1;
	unsigned int bloomFBO2;
	unsigned int depthMapFBO;
	unsigned int omniDepthMapFBO;
	
	unsigned int textureColorBuffer1;
	unsigned int textureColorBuffer2;
	unsigned int shadowColorBuffer1;
	unsigned int shadowColorBuffer2;
	unsigned int bloomColorBuffer1;
	unsigned int bloomColorBuffer2;

	unsigned int depthStencilBuffer1;
	unsigned int depthStencilBuffer2;
	unsigned int shadowDepthStencilBuffer1;
	unsigned int shadowDepthStencilBuffer2;
	unsigned int bloomDepthStencilBuffer1;
	unsigned int bloomDepthStencilBuffer2;

	unsigned int stencilView1;
	unsigned int stencilView2;
	unsigned int shadowStencilView1;
	unsigned int shadowStencilView2;
	unsigned int bloomStencilView1;
	unsigned int bloomStencilView2;

	unsigned int depthMap;
	unsigned int omniDepthMap;

	// g buffer & its textures
	unsigned int gBuffer;
	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int gAlbedo;
	unsigned int gSpecular;
	unsigned int gDepth;
	unsigned int gStencil;

	// debug materials
	Material* normalDebugMaterial;
	Material* orientationDebugMaterial;

	// special post process materials
	std::shared_ptr<PostProcessMaterial> brightPixelsExtractionMaterial; // used for bloom, gets the texture with pixels with brightness over bloom threshold
	std::shared_ptr<PostProcessMaterial> bloomBlurMaterial;
	std::shared_ptr<PostProcessMaterial> hdrToneMappingGammaCorrectionMaterial;
	std::shared_ptr<PostProcessMaterial> editorOutlineMaterial;
	std::shared_ptr<PostProcessMaterial> deferredShadingMaterial;
	std::shared_ptr<PostProcessMaterial> textureMergerMaterial;

	// render pipeline materials
	Material* depthMapMaterial;
	Material* omniDepthMapMaterial;
};