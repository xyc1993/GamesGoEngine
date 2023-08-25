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

enum class AntiAliasingAlgorithm
{
	FXAA
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
	void InitSSAOData();
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
	void ConfigureSSAOBuffers(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer);
	void ConfigureBloomBuffer(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer);

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
	void UpdateSSAOBuffers();
	void UpdateDirectionalShadowMap(Light* directionalLight, glm::mat4& lightSpaceMatrix);
	void UpdateSpotLightShadowMap(Light* spotLight, glm::mat4& lightSpaceMatrix);
	void UpdateOmnidirectionalShadowMap(Light* pointLight);
	bool AreThereAnyShadowCasters() const;
	void DrawDeferredShadedObjects();
	void DrawForwardShadedObjects();
	void DrawOrientationDebug() const;
	void DrawSkybox();
	void DrawDebug();
	void DrawScreenEffects();
	static void DrawRenderers(const std::vector<MeshRenderer*>& renderers);
	static void DrawRenderersOfLightModel(const std::vector<MeshRenderer*>& renderers, LightModelType lightModel);
	static void DrawRenderersExceptLightModel(const std::vector<MeshRenderer*>& renderers, LightModelType lightModel);
	static void DrawRenderers(const std::vector<MeshRenderer*>& renderers, Material* material);
	static void DrawShadowCastingRenderers(const std::vector<MeshRenderer*>& renderers, Material* material);
	static void DrawPostProcessEffects();
	static void ApplyLightForRenderers(Light* light, const std::vector<MeshRenderer*>& renderers);
	void ApplyAllLightDataForForwardRenderers();
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
	static void SetBloomStrength(float amount);
	static float GetBloomStrength();
	static void SetBloomMipChainLength(unsigned int length);
	static unsigned int GetBloomMipChainLength();
	// assume square textures
	static void SetShadowMapResolution(unsigned int shadowMapRes);
	static unsigned int GetShadowMapResolution();
private:
	void SetShadowMapResolutionInternal(unsigned int shadowMapRes);
public:
	static void EnableSSAO(bool enable);
	static bool IsSSAOEnabled();
private:
	void EnableSSAOInternal(bool enable);
public:
	static void EnableAntiAliasing(bool enable);
	static bool IsAntiAliasingEnabled();
	static void SetAntiAliasingAlgorithm(AntiAliasingAlgorithm algorithm);
	static AntiAliasingAlgorithm GetAntiAliasingAlgorithm();
	static float GetFXAAContrastThreshold();
	static void SetFXAAContrastThreshold(float contrastThreshold);
	static float GetFXAARelativeThreshold();
	static void SetFXAARelativeThreshold(float relativeThreshold);
	
private:
	static bool CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);
	static bool CompareTransparentRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2);
	static bool ComparePostProcessMaterialsPositions(const std::shared_ptr<PostProcessMaterial>& ppm1, const std::shared_ptr<PostProcessMaterial>& ppm2);

	bool AreThereAnyDeferredRendereredMeshes() const;
	bool AreThereAnyForwardRendereredMeshes() const;
	size_t GetRenderersNumberOfType(LightModelType lightModel) const;

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
	bool hdrTonemappingAndGamma = true;
	bool firstRenderedFrame = true;
	bool renderWireframeOnly = false;
	bool normalsDebugEnabled = false;
	float gamma = 2.2f;
	float exposure = 1.0f;
	unsigned int shadowWidth = 1024;
	unsigned int shadowHeight = 1024;
	bool ssaoEnabled = true;
	bool aaEnabled = true;
	AntiAliasingAlgorithm aaAlgorithm;
	float fxaaContrastThreshold = 0.0312f;
	float fxaaRelativeThreshold = 0.063f;

	// uniform buffers used globally by shaders
	unsigned int uboMatrices;
	unsigned int uboCameraData;
	unsigned int uboTimeData;

	// sets of 2 since we use ping pong rendering
	unsigned int framebuffer1;
	unsigned int framebuffer2;
	unsigned int shadowFBO1;
	unsigned int shadowFBO2;
	unsigned int directionalDepthMapFBO;
	unsigned int spotLightDepthMapFBO;
	unsigned int omniDepthMapFBO;
	
	unsigned int textureColorBuffer1;
	unsigned int textureColorBuffer2;
	unsigned int shadowColorBuffer1;
	unsigned int shadowColorBuffer2;

	unsigned int depthStencilBuffer1;
	unsigned int depthStencilBuffer2;
	unsigned int shadowDepthStencilBuffer1;
	unsigned int shadowDepthStencilBuffer2;

	unsigned int stencilView1;
	unsigned int stencilView2;
	unsigned int shadowStencilView1;
	unsigned int shadowStencilView2;

	unsigned int directionalDepthMap;
	unsigned int spotLightDepthMap;
	unsigned int omniDepthMap;

	// g buffer & its textures
	unsigned int gBuffer;
	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int gAlbedo;
	unsigned int gMetallic;
	unsigned int gRoughness;
	unsigned int gAmbientOcclusion;
	unsigned int gEmissive;
	unsigned int gDepth;
	unsigned int gStencil;

	// SSAO buffers, textures and other data
	unsigned int ssaoFBO;
	unsigned int ssaoBlurFBO;
	unsigned int ssaoColorBuffer;
	unsigned int ssaoColorBufferBlur;
	unsigned int ssaoNoiseTexture;
	std::vector<glm::vec3> ssaoKernel;
	const unsigned int ssaoKernelSize = 64;

	// bloom buffer and other bloom related data
	struct BloomMip
	{
		glm::vec2 size;
		glm::ivec2 intSize;
		unsigned int texture;
	};
	unsigned int bloomFBO;
	bool bloomEnabled = true;
	float bloomStrength = 0.04f;
	unsigned int bloomMipChainLength = 6;
	std::vector<BloomMip> bloomMipChain;

	// debug materials
	Material* normalDebugMaterial;
	Material* orientationDebugMaterial;

	// special post process materials
	std::shared_ptr<PostProcessMaterial> hdrToneMappingGammaCorrectionMaterial;
	std::shared_ptr<PostProcessMaterial> editorOutlineMaterial;
	std::shared_ptr<PostProcessMaterial> deferredShadingMaterial;
	std::shared_ptr<PostProcessMaterial> deferredAmbientLightShadowedAdditiveMaterial;
	std::shared_ptr<PostProcessMaterial> deferredDirectionalLightShadowedAdditiveMaterial;
	std::shared_ptr<PostProcessMaterial> deferredPointLightShadowedAdditiveMaterial;
	std::shared_ptr<PostProcessMaterial> deferredSpotLightShadowedAdditiveMaterial;
	std::shared_ptr<PostProcessMaterial> textureMergerMaterial;
	std::shared_ptr<PostProcessMaterial> ssaoMaterial;
	std::shared_ptr<PostProcessMaterial> ssaoBlurMaterial;
	std::shared_ptr<PostProcessMaterial> fxaaMaterial;
	std::shared_ptr<PostProcessMaterial> bloomDownsampleMaterial;
	std::shared_ptr<PostProcessMaterial> bloomUpsampleMaterial;

	// render pipeline materials
	Material* depthMapMaterial;
	Material* omniDepthMapMaterial;
};