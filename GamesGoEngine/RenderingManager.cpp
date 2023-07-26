#include "RenderingManager.h"

#include <algorithm>

#include "CamerasManager.h"
#include "GameObject.h"
#include "Math.h"
#include "MeshPrimitivesPool.h"
#include "MeshRenderer.h"
#include "PostProcessMaterial.h"
#include "PostProcessRenderer.h"
#include "Time.h"
#include "WindowManager.h"

RenderingManager* RenderingManager::instance = nullptr;

RenderingManager::RenderingManager()
{
	lightsManager = new LightsManager();

	//debug materials
	normalDebugMaterial = new Material("res/shaders/debugNormals.vert.glsl", "res/shaders/debugNormals.frag.glsl", "res/shaders/debugNormals.geom.glsl");
	orientationDebugMaterial = new Material("res/shaders/DebugShaders/debugOrientation.vert.glsl", "res/shaders/DebugShaders/debugOrientation.frag.glsl", "res/shaders/DebugShaders/debugOrientation.geom.glsl");

	//render pipeline materials
	depthMapMaterial = new Material("res/shaders/RenderPipeline/depthMap.vert.glsl", "res/shaders/RenderPipeline/depthMap.frag.glsl");
	omniDepthMapMaterial = new Material("res/shaders/RenderPipeline/omniDepthMap.vert.glsl", "res/shaders/RenderPipeline/omniDepthMap.frag.glsl", "res/shaders/RenderPipeline/omniDepthMap.geom.glsl");

	// post process related materials
	brightPixelsExtractionMaterial = std::make_shared<PostProcessMaterial>("res/shaders/RenderPipeline/extractBrightColor.frag.glsl");
	bloomBlurMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/blurGaussian.frag.glsl");
	hdrToneMappingGammaCorrectionMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/hdrToneMappingGammaCorrection.frag.glsl");
	editorOutlineMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/editorOutline.frag.glsl");
	deferredShadingMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/deferredShadingSimple.frag.glsl");
	textureMergerMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/textureMerger.frag.glsl");
}

RenderingManager::~RenderingManager()
{
	delete lightsManager;
	delete normalDebugMaterial;
	delete depthMapMaterial;
	delete omniDepthMapMaterial;
}

RenderingManager* RenderingManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new RenderingManager();
	}
	return instance;
}

void RenderingManager::Init(GLint screenWidth, GLint screenHeight)
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	GetInstance()->ConfigureFramebuffers(screenWidth, screenHeight, true);
	GetInstance()->ConfigureUniformBufferObjects();
	GetInstance()->CreateDebugMaterials();
	GetInstance()->InitGammaCorrection();
}

void RenderingManager::InitGammaCorrection()
{
	// make sure that the default gamma value & exposure is set in the material
	SetGamma(GetGamma());
	SetExposure(GetExposure());
}

void RenderingManager::ConfigureFramebuffers(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer)
{
	ConfigureFramebuffer(screenWidth, screenHeight, framebuffer1, textureColorBuffer1, depthStencilBuffer1, stencilView1, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, framebuffer2, textureColorBuffer2, depthStencilBuffer2, stencilView2, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, shadowFBO1, shadowColorBuffer1, shadowDepthStencilBuffer1, shadowStencilView1, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, shadowFBO2, shadowColorBuffer2, shadowDepthStencilBuffer2, shadowStencilView2, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, bloomFBO1, bloomColorBuffer1, bloomDepthStencilBuffer1, bloomStencilView1, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, bloomFBO2, bloomColorBuffer2, bloomDepthStencilBuffer2, bloomStencilView2, shouldGenerateFramebuffer);
	ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, directionalDepthMapFBO, directionalDepthMap, shouldGenerateFramebuffer);
	ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, spotLightDepthMapFBO, spotLightDepthMap, shouldGenerateFramebuffer);
	ConfigureShadowCubeMapFramebuffer(shadowWidth, shadowHeight, omniDepthMapFBO, omniDepthMap, shouldGenerateFramebuffer);
	ConfigureGBuffer(screenWidth, screenHeight, shouldGenerateFramebuffer);
}

void RenderingManager::ConfigureFramebuffer(GLint screenWidth, GLint screenHeight,
	unsigned int& framebuffer, unsigned int& textureColorBuffer,
	unsigned int& depthStencilBuffer, unsigned int& stencilView,
	bool shouldGenerateFramebuffer)
{
	if (shouldGenerateFramebuffer)
	{
		glGenFramebuffers(1, &framebuffer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	// create a depth & stencil attachment texture
	glGenTextures(1, &depthStencilBuffer);
	glBindTexture(GL_TEXTURE_2D, depthStencilBuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
	
	glGenTextures(1, &stencilView);
	glTextureView(stencilView, GL_TEXTURE_2D, depthStencilBuffer, GL_DEPTH24_STENCIL8, 0, 1, 0, 1);

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingManager::ConfigureShadowMapFramebuffer(GLint shadowMapWidth, GLint shadowMapHeight,
	unsigned& framebuffer, unsigned& shadowMap, bool shouldGenerateFramebuffer)
{
	if (shouldGenerateFramebuffer)
	{
		glGenFramebuffers(1, &framebuffer);
	}

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// ensure that the objects outside of map are not in shadow
	constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// no color data needed for depth so disable draw & read buffers in the framebuffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingManager::ConfigureShadowCubeMapFramebuffer(GLint shadowMapWidth, GLint shadowMapHeight,
	unsigned& framebuffer, unsigned& shadowCubeMap, bool shouldGenerateFramebuffer)
{
	if (shouldGenerateFramebuffer)
	{
		glGenFramebuffers(1, &framebuffer);
	}
	
	glGenTextures(1, &shadowCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}	

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingManager::ConfigureGBuffer(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer)
{
	if (shouldGenerateFramebuffer)
	{
		glGenFramebuffers(1, &gBuffer);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	// specular color buffer
	glGenTextures(1, &gSpecular);
	glBindTexture(GL_TEXTURE_2D, gSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gSpecular, 0);
	// light enabled color buffer
	glGenTextures(1, &gLightEnabled);
	glBindTexture(GL_TEXTURE_2D, gLightEnabled);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gLightEnabled, 0);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments);

	// create a depth & stencil attachment texture
	glGenTextures(1, &gDepth);
	glBindTexture(GL_TEXTURE_2D, gDepth);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

	glGenTextures(1, &gStencil);
	glTextureView(gStencil, GL_TEXTURE_2D, gDepth, GL_DEPTH24_STENCIL8, 0, 1, 0, 1);

	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingManager::ResizeBuffers(GLint screenWidth, GLint screenHeight)
{
	GetInstance()->ResizeBuffersInternal(screenWidth, screenHeight);
}

void RenderingManager::ResizeBuffersInternal(GLint screenWidth, GLint screenHeight)
{
	// not the most lightweight approach since we need to recreate textures but it is required due to how we write data from stencil buffer into texture
	glDeleteTextures(1, &textureColorBuffer1);
	glDeleteTextures(1, &textureColorBuffer2);
	glDeleteTextures(1, &shadowColorBuffer1);
	glDeleteTextures(1, &shadowColorBuffer2);
	glDeleteTextures(1, &bloomColorBuffer1);
	glDeleteTextures(1, &bloomColorBuffer2);

	glDeleteTextures(1, &depthStencilBuffer1);
	glDeleteTextures(1, &depthStencilBuffer2);
	glDeleteTextures(1, &shadowDepthStencilBuffer1);
	glDeleteTextures(1, &shadowDepthStencilBuffer2);
	glDeleteTextures(1, &bloomDepthStencilBuffer1);
	glDeleteTextures(1, &bloomDepthStencilBuffer2);

	glDeleteTextures(1, &stencilView1);
	glDeleteTextures(1, &stencilView2);
	glDeleteTextures(1, &shadowStencilView1);
	glDeleteTextures(1, &shadowStencilView2);
	glDeleteTextures(1, &bloomStencilView1);
	glDeleteTextures(1, &bloomStencilView2);

	glDeleteTextures(1, &directionalDepthMap);
	glDeleteTextures(1, &spotLightDepthMap);
	glDeleteTextures(1, &omniDepthMap);

	glDeleteTextures(1, &gPosition);
	glDeleteTextures(1, &gNormal);
	glDeleteTextures(1, &gAlbedo);
	glDeleteTextures(1, &gSpecular);
	glDeleteTextures(1, &gLightEnabled);
	glDeleteTextures(1, &gDepth);
	glDeleteTextures(1, &gStencil);

	GetInstance()->ConfigureFramebuffers(screenWidth, screenHeight, false);
}

void RenderingManager::ConfigureUniformBufferObjects()
{
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	glGenBuffers(1, &uboCameraData);
	glBindBuffer(GL_UNIFORM_BUFFER, uboCameraData);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboCameraData, 0, 2 * sizeof(glm::vec3));

	glGenBuffers(1, &uboTimeData);
	glBindBuffer(GL_UNIFORM_BUFFER, uboTimeData);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboTimeData, 0, 2 * sizeof(float));
}

void RenderingManager::CreateDebugMaterials()
{

}

void RenderingManager::Update()
{
	// update uniform buffer objects
	GetInstance()->UpdateUniformBufferObjects();

	// TODO: more optimal sorting, it could sort on camera view change, not on every draw frame
	SortTransparentMeshRenderers();
	
	// Rendering the scene
	// set viewport
	glViewport(0, 0, WindowManager::GetCurrentWidth(), WindowManager::GetCurrentHeight());

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glEnable(GL_STENCIL_TEST);

	GetInstance()->UpdateGBuffer();

	glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->framebuffer2);

	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glStencilMask(~0);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	// disable depth and buffer for deferred shading rendering since it's quad rendering over whole screen
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	const int width = WindowManager::GetCurrentWidth();
	const int height = WindowManager::GetCurrentHeight();

	// deferred rendering
	// TODO: check if there are any deferred renderers at all (otherwise skip this)
	GetInstance()->deferredShadingMaterial->SetTexture("gPosition", 0, GetInstance()->gPosition);
	GetInstance()->deferredShadingMaterial->SetTexture("gNormal", 1, GetInstance()->gNormal);
	GetInstance()->deferredShadingMaterial->SetTexture("gAlbedo", 2, GetInstance()->gAlbedo);
	GetInstance()->deferredShadingMaterial->SetTexture("gSpecular", 3, GetInstance()->gSpecular);
	GetInstance()->deferredShadingMaterial->SetTexture("gLightEnabled", 4, GetInstance()->gLightEnabled);
	GetInstance()->UpdateDeferredShading();
	GetInstance()->deferredShadingMaterial->Draw(glm::mat4());
	MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glEnable(GL_STENCIL_TEST);
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GetInstance()->gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GetInstance()->framebuffer2);
	
	// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
	// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
	// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
	// color texture is GL_COLOR_BUFFER_BIT + 2 (there are 4 buffers in the following order: positions, normals, color, specular)
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT + 2, GL_NEAREST);
	// TODO: make it work with shadowmapping, currently causes weird artifacts, perhaps due to no data written in gBuffer in tested map
	// TODO: make sure deferred rendering will work before commiting, right now it's BROKEN!
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	
	glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->framebuffer2);

	/* TEMPORARILY DISABLED WHILE WORKING ON DEFERRED RENDERING
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GetInstance()->framebuffer2);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GetInstance()->framebuffer1);

	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// forward rendering with shadows
	// TODO: check if there are any forward renderers at all (otherwise skip this)
	std::vector<Light*> lights = GetLightsManager()->GetAllLights();
	const bool areThereAnyShadowCasters = GetInstance()->AreThereAnyShadowCasters();
	if (!lights.empty() && areThereAnyShadowCasters)
	{
		for (size_t i = 0; i < lights.size(); i++)
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_STENCIL_TEST);

			DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(lights[i]);
			if (directionalLight != nullptr)
			{
				GetInstance()->UpdateDirectionalShadowMap(directionalLight);
			}
			PointLight* pointLight = dynamic_cast<PointLight*>(lights[i]);
			if (pointLight != nullptr)
			{
				GetInstance()->UpdateOmnidirectionalShadowMap(pointLight);
			}
			SpotLight* spotLight = dynamic_cast<SpotLight*>(lights[i]);
			if (spotLight != nullptr)
			{
				GetInstance()->UpdateSpotLightShadowMap(spotLight);
			}
			
			// reset viewport
			glViewport(0, 0, WindowManager::GetCurrentWidth(), WindowManager::GetCurrentHeight());
			glBindFramebuffer(GL_FRAMEBUFFER, i % 2 == 0 ? GetInstance()->framebuffer2 : GetInstance()->framebuffer1);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			ApplyLightForRenderers(lights[i], GetInstance()->opaqueMeshRenderers);
			ApplyLightForRenderers(lights[i], GetInstance()->transparentMeshRenderers);

			DrawRenderersExceptLightModel(GetInstance()->opaqueMeshRenderers, LightModelType::LitDeferred);
			DrawRenderersExceptLightModel(GetInstance()->transparentMeshRenderers, LightModelType::LitDeferred);

			if (i > 0)
			{
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_STENCIL_TEST);

				glBindFramebuffer(GL_FRAMEBUFFER, i % 2 == 0 ? GetInstance()->shadowFBO2 : GetInstance()->shadowFBO1);
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// merge last texture and sum of previous textures
				// set latest texture
				GetInstance()->textureMergerMaterial->SetTexture("screenTexture1", 0, i % 2 == 0 ? GetInstance()->textureColorBuffer2 : GetInstance()->textureColorBuffer1);
				// set sum of textures
				if (i == 1) // for i equal 1 we use the first texture to sum first and second texture
				{
					GetInstance()->textureMergerMaterial->SetTexture("screenTexture2", 1, GetInstance()->textureColorBuffer2);
				}
				else // otherwise we use sum
				{
					GetInstance()->textureMergerMaterial->SetTexture("screenTexture2", 1, i % 2 == 0 ? GetInstance()->shadowColorBuffer1 : GetInstance()->shadowColorBuffer2);
				}
				
				GetInstance()->textureMergerMaterial->Draw(glm::mat4());
				MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
			}

			// clear lights so the next light pass won't be affected by previous one
			ClearLightsForRenderers(GetInstance()->opaqueMeshRenderers);
			ClearLightsForRenderers(GetInstance()->transparentMeshRenderers);
		}

		if (lights.size() > 1)
		{
			// after rendering the last shadow pass, blit data to the framebuffer that's used later for the rest of rendering process
			glBindFramebuffer(GL_READ_FRAMEBUFFER, lights.size() % 2 == 0 ? GetInstance()->shadowFBO1 : GetInstance()->shadowFBO2);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GetInstance()->framebuffer2);

			// blit only color
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			//glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
			//glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_STENCIL_TEST);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->framebuffer2);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->framebuffer2);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GetInstance()->ApplyAllLightDataForForwardRenderers();
		DrawRenderersExceptLightModel(GetInstance()->opaqueMeshRenderers, LightModelType::LitDeferred);
		DrawRenderersExceptLightModel(GetInstance()->transparentMeshRenderers, LightModelType::LitDeferred);
	}
	*/
	DrawSkybox();

	if (IsNormalsDebugEnabled() && (GetInstance()->normalDebugMaterial != nullptr))
	{
		DrawRenderers(GetInstance()->meshRenderers, GetInstance()->normalDebugMaterial);
	}

	GetInstance()->DrawOrientationDebug();

	if (GetInstance()->firstRenderedFrame)
	{
		GetInstance()->firstRenderedFrame = false;
		SortPostProcessMaterials();
	}

	// pass the framebuffer2 (the one we worked on so far) to framebuffer1 so both have same stencil and depth data (used by some screen effects)
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GetInstance()->framebuffer2);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GetInstance()->framebuffer1);

	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	// Post process effects
	const size_t postProcessEffects = GetInstance()->usedPostProcessMaterials.size();
	if (IsPostProcessingEnabled())
	{
		// post processing via ping pong rendering
		for (size_t i = 0; i < postProcessEffects; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, i % 2 == 0 ? GetInstance()->framebuffer2 : GetInstance()->framebuffer1);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			const std::shared_ptr<Material> ppMaterial = GetInstance()->usedPostProcessMaterials[i];
			ppMaterial->SetTexture("screenTexture", 0, i % 2 == 0 ? GetInstance()->textureColorBuffer1 : GetInstance()->textureColorBuffer2);
			ppMaterial->SetTexture("depthStencilTexture", 1, i % 2 == 0 ? GetInstance()->depthStencilBuffer1 : GetInstance()->depthStencilBuffer2);
			ppMaterial->SetTexture("stencilView", 2, i % 2 == 0 ? GetInstance()->stencilView1 : GetInstance()->stencilView2);
			ppMaterial->Draw(glm::mat4());
			MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
		}
	}
	int lastPostProcessMaterialIndex = postProcessEffects;

	// Bloom
	if (IsBloomEnabled())
	{
		// extract pixels over bloom's brightness threshold	
		glBindFramebuffer(GL_FRAMEBUFFER, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->framebuffer2 : GetInstance()->framebuffer1);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GetInstance()->brightPixelsExtractionMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->textureColorBuffer1 : GetInstance()->textureColorBuffer2);
		GetInstance()->brightPixelsExtractionMaterial->SetTexture("depthStencilTexture", 1, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->depthStencilBuffer1 : GetInstance()->depthStencilBuffer2);
		GetInstance()->brightPixelsExtractionMaterial->SetTexture("stencilView", 2, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->stencilView1 : GetInstance()->stencilView2);
		GetInstance()->brightPixelsExtractionMaterial->Draw(glm::mat4());
		MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);

		// blur pixels meant for blur
		bool horizontal = true;
		bool firstIteration = true;
		// get texture after bright pixels extraction
		lastPostProcessMaterialIndex++;
		for (unsigned int i = 0; i < GetInstance()->bloomBlurAmount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, horizontal ? GetInstance()->bloomFBO1 : GetInstance()->bloomFBO2);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			GetInstance()->bloomBlurMaterial->SetInt("horizontal", horizontal);
			if (firstIteration)
			{
				GetInstance()->bloomBlurMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->textureColorBuffer1 : GetInstance()->textureColorBuffer2);
			}
			else
			{
				GetInstance()->bloomBlurMaterial->SetTexture("screenTexture", 0, i % 2 == 1 ? GetInstance()->bloomColorBuffer1 : GetInstance()->bloomColorBuffer2);
			}
			GetInstance()->bloomBlurMaterial->Draw(glm::mat4());
			MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
			horizontal = !horizontal;
			if (firstIteration)
			{
				firstIteration = false;
			}
		}
	}
	
	// HDR tone mapping and gamma correction
	if (IsBloomEnabled())
	{
		// get texture before bright pixels extraction
		lastPostProcessMaterialIndex--;
	}	
	if (IsHDRToneMappingAndGammaEnabled())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->framebuffer2 : GetInstance()->framebuffer1);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->textureColorBuffer1 : GetInstance()->textureColorBuffer2);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetTexture("depthStencilTexture", 1, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->depthStencilBuffer1 : GetInstance()->depthStencilBuffer2);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetTexture("stencilView", 2, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->stencilView1 : GetInstance()->stencilView2);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetTexture("bloomBlur", 3, (GetInstance()->bloomBlurAmount - 1) % 2 == 1 ? GetInstance()->bloomColorBuffer1 : GetInstance()->bloomColorBuffer2);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetFloat("bloomEnabled", IsBloomEnabled() ? 1.0f : 0.0f);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->Draw(glm::mat4());
		MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
	}

	// lastly apply selection outline, saved for the last so it wouldn't be altered by post processing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (IsHDRToneMappingAndGammaEnabled())
	{
		lastPostProcessMaterialIndex++;
	}
	GetInstance()->editorOutlineMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->textureColorBuffer1 : GetInstance()->textureColorBuffer2);
	GetInstance()->editorOutlineMaterial->SetTexture("depthStencilTexture", 1, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->depthStencilBuffer1 : GetInstance()->depthStencilBuffer2);
	GetInstance()->editorOutlineMaterial->SetTexture("stencilView", 2, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->stencilView1 : GetInstance()->stencilView2);
	GetInstance()->editorOutlineMaterial->Draw(glm::mat4());
	MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
}

void RenderingManager::UpdateUniformBufferObjects()
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(CamerasManager::GetActiveCameraProjectionMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(CamerasManager::GetActiveCameraViewMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboCameraData);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(CamerasManager::GetActiveCameraPosition()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(glm::vec3), glm::value_ptr(CamerasManager::GetActiveCameraDirection()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboTimeData);
	const auto time = static_cast<float>(Time::GetTime());
	const auto deltaTime = static_cast<float>(Time::GetDeltaTime());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &time);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float), sizeof(float), &deltaTime);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderingManager::UpdateGBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawRenderersOfLightModel(meshRenderers, LightModelType::LitDeferred);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderingManager::UpdateDeferredShading()
{
	const size_t pointLightsNumber = GetLightsManager()->GetPointLightsNumber();
	deferredShadingMaterial->SetInt("pointLightsNumber", pointLightsNumber);
	for (size_t i = 0; i < pointLightsNumber; i++)
	{
		Light* light = GetLightsManager()->GetPointLight(i);
		PointLight* pointLight = (PointLight*)light;
		pointLight->SetLightInShader(deferredShadingMaterial->GetShaderProgram());
	}
}

void RenderingManager::UpdateDirectionalShadowMap(Light* directionalLight)
{
	if (!Component::IsValid(directionalLight))
	{
		return;
	}
	
	float near_plane = 1.0f, far_plane = 30.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, 10.0f, -10.0f, near_plane, far_plane);

	glm::mat4 lightSpaceMatrix;
	Transform* lightTransform = directionalLight->GetOwner()->GetTransform();
	glm::vec3 lightPosition = lightTransform->GetPosition() - 10.0f * lightTransform->GetForward();
	glm::vec3 lightLookAtTarget = lightTransform->GetPosition() + 10.0f * lightTransform->GetForward();
	glm::mat4 lightView = glm::lookAt(lightPosition, lightLookAtTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
	depthMapMaterial->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, directionalDepthMapFBO);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	DrawShadowCastingRenderers(meshRenderers, depthMapMaterial);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// TODO: remove and implement more elegant solution, this is not universal solution, just one for specific example scene
	for (size_t i = 0; i < meshRenderers.size(); i++)
	{
		std::shared_ptr<Material> outMaterial;
		if (meshRenderers[i]->TryGetMaterial(outMaterial, 0))
		{
			outMaterial->SetMat4("directionalLightSpaceMatrix", lightSpaceMatrix);
			outMaterial->SetTexture("directionalLightShadowMap", 0, directionalDepthMap);
		}
	}
}

void RenderingManager::UpdateSpotLightShadowMap(Light* spotLight)
{
	if (!Component::IsValid(spotLight))
	{
		return;
	}

	float near_plane = 0.1f, far_plane = 30.0f;
	// TODO: calculate fov and aspect properly to adjust to the value of spot light
	glm::mat4 lightProjection = glm::perspective(40.0f, 1.0f, near_plane, far_plane);

	glm::mat4 lightSpaceMatrix;
	Transform* lightTransform = spotLight->GetOwner()->GetTransform();
	glm::vec3 lightPosition = lightTransform->GetPosition();
	glm::vec3 lightLookAtTarget = lightPosition + 10.0f * lightTransform->GetForward();
	glm::mat4 lightView = glm::lookAt(lightPosition, lightLookAtTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
	depthMapMaterial->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, spotLightDepthMapFBO);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	DrawShadowCastingRenderers(meshRenderers, depthMapMaterial);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// TODO: remove and implement more elegant solution, this is not universal solution, just one for specific example scene
	for (size_t i = 0; i < meshRenderers.size(); i++)
	{
		std::shared_ptr<Material> outMaterial;
		if (meshRenderers[i]->TryGetMaterial(outMaterial, 0))
		{
			outMaterial->SetMat4("spotLightSpaceMatrix", lightSpaceMatrix);
			outMaterial->SetTexture("spotLightShadowMap", 1, spotLightDepthMap);
		}
	}
}

void RenderingManager::UpdateOmnidirectionalShadowMap(Light* pointLight)
{
	if (!Component::IsValid(pointLight))
	{
		return;
	}

	float aspect = (float)shadowWidth / (float)shadowHeight;
	float near = 0.1f;
	float far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	glm::vec3 lightPos = pointLight->GetOwner()->GetTransform()->GetPosition();

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, omniDepthMapFBO);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	//glCullFace(GL_FRONT);
	for (unsigned int i = 0; i < 6; ++i)
	{
		std::stringstream stream;
		std::string matrixName;
		stream << "shadowMatrices[" << std::to_string(i) << "]";
		stream >> matrixName;
		omniDepthMapMaterial->SetMat4(matrixName.c_str(), shadowTransforms[i]);
	}
	omniDepthMapMaterial->SetFloat("far_plane", far);
	omniDepthMapMaterial->SetVector3("lightPos", lightPos);
	DrawShadowCastingRenderers(meshRenderers, omniDepthMapMaterial);
	//glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// TODO: remove and implement more elegant solution, this is not universal solution, just one for specific example scene
	for (size_t i = 0; i < meshRenderers.size(); i++)
	{
		std::shared_ptr<Material> outMaterial;
		if (meshRenderers[i]->TryGetMaterial(outMaterial, 0))
		{
			outMaterial->SetCubeTexture("pointLightShadowMap", 2, omniDepthMap);
			outMaterial->SetFloat("far_plane", far);
			// Legacy feature for the normal/parallax mapping shaders
			outMaterial->SetVector3("lightPos", lightPos);
		}
	}
}

// TODO: since it's not optimal to do this every frame, it should be cached, values can be updated on "SetIsCastinShadow(...)" call
bool RenderingManager::AreThereAnyShadowCasters() const
{
	for (size_t i = 0; i < meshRenderers.size(); i++)
	{
		if (meshRenderers[i]->IsCastingShadow())
		{
			return true;
		}
	}
	return false;
}

void RenderingManager::DrawOrientationDebug() const
{
	if (selectedGameObject != nullptr)
	{
		const Transform* transform = selectedGameObject->GetTransform();
		const glm::mat4 transformMatrix = transform->GetTransformMatrix();
		const glm::vec3 forward = transform->GetForward();
		const glm::vec3 up = transform->GetUp();
		const glm::vec3 right = transform->GetRight();
		const glm::vec3 position = transform->GetPosition();

		const GLfloat vertexPosition[3] = { position.x, position.y, position.z };

		orientationDebugMaterial->SetVector3("forward", forward);
		orientationDebugMaterial->SetVector3("up", up);
		orientationDebugMaterial->SetVector3("right", right);
		orientationDebugMaterial->Draw(transformMatrix);

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}
}

void RenderingManager::DrawSkybox()
{
	if (GetInstance()->skybox != nullptr)
	{
		GetInstance()->skybox->Draw();
	}
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

void RenderingManager::DrawRenderersOfLightModel(const std::vector<MeshRenderer*>& renderers, LightModelType lightModel)
{
	for (size_t i = 0; i < renderers.size(); i++)
	{
		MeshRenderer* meshRenderer = renderers[i];
		if (meshRenderer != nullptr && meshRenderer->GetLightModelType() == lightModel)
		{
			meshRenderer->Draw();
		}
	}
}

void RenderingManager::DrawRenderersExceptLightModel(const std::vector<MeshRenderer*>& renderers,
	LightModelType lightModel)
{
	for (size_t i = 0; i < renderers.size(); i++)
	{
		MeshRenderer* meshRenderer = renderers[i];
		if (meshRenderer != nullptr && meshRenderer->GetLightModelType() != lightModel)
		{
			meshRenderer->Draw();
		}
	}
}

void RenderingManager::DrawRenderers(const std::vector<MeshRenderer*>& renderers, Material* material)
{
	for (size_t i = 0; i < renderers.size(); i++)
	{
		MeshRenderer* meshRenderer = renderers[i];
		if (meshRenderer != nullptr)
		{
			meshRenderer->Draw(material);
		}
	}
}

void RenderingManager::DrawShadowCastingRenderers(const std::vector<MeshRenderer*>& renderers, Material* material)
{
	for (size_t i = 0; i < renderers.size(); i++)
	{
		MeshRenderer* meshRenderer = renderers[i];
		if ((meshRenderer != nullptr) && (meshRenderer->IsCastingShadow()))
		{
			meshRenderer->Draw(material);
		}
	}
}

void RenderingManager::DrawPostProcessEffects()
{

}

void RenderingManager::ApplyLightForRenderers(Light* light, const std::vector<MeshRenderer*>& renderers)
{
	for (size_t i = 0; i < renderers.size(); i++)
	{
		for (size_t j = 0; j < renderers[i]->materialList.size(); j++)
		{
			light->SetThisLightInShader(renderers[i]->materialList[j]->GetShaderProgram());
		}
	}
}

void RenderingManager::ApplyAllLightDataForForwardRenderers()
{
	for (size_t i = 0; i < meshRenderers.size(); i++)
	{
		if (meshRenderers[i]->GetLightModelType() == LightModelType::LitForward)
		{
			for (size_t j = 0; j < meshRenderers[i]->materialList.size(); j++)
			{
				const GLuint shaderProgram = meshRenderers[i]->materialList[j]->GetShaderProgram();
				glUseProgram(shaderProgram);
				GetLightsManager()->SetLightsInShader(shaderProgram);
			}
		}
	}
}

void RenderingManager::ClearLightsForRenderers(const std::vector<MeshRenderer*>& renderers)
{
	for (size_t i = 0; i < renderers.size(); i++)
	{
		for (size_t j = 0; j < renderers[i]->materialList.size(); j++)
		{
			const GLuint shaderProgram = renderers[i]->materialList[j]->GetShaderProgram();
			glUseProgram(shaderProgram);
			glUniform1f(glGetUniformLocation(shaderProgram, "ambientLightActive"), 0.0f);
			glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), 0);
			glUniform1i(glGetUniformLocation(shaderProgram, "pointLightsNumber"), 0);

			// TODO: think how to make it more generic since some shaders have different texture indices and this could overwrite their textures
			// always make sure that textures are set, even when they're not used; textures must be bound and complete			
			renderers[i]->materialList[j]->SetTexture("directionalLightShadowMap", 0, GetInstance()->directionalDepthMap);
			renderers[i]->materialList[j]->SetTexture("spotLightShadowMap", 1, GetInstance()->spotLightDepthMap);
			renderers[i]->materialList[j]->SetCubeTexture("pointLightShadowMap", 2, GetInstance()->omniDepthMap);
		}
	}
}

void RenderingManager::SetSkybox(SkyboxRenderer* skybox)
{
	if (GetInstance()->skybox != nullptr)
	{
		delete GetInstance()->skybox;
	}
	GetInstance()->skybox = skybox;
}

void RenderingManager::AddMeshRenderer(MeshRenderer* meshRenderer)
{
	GetInstance()->meshRenderers.push_back(meshRenderer);
	SortMeshRenderers();
}

void RenderingManager::RemoveMeshRenderer(MeshRenderer* meshRenderer)
{
	for (size_t i = 0; i < GetInstance()->meshRenderers.size(); i++)
	{
		if (GetInstance()->meshRenderers[i] == meshRenderer)
		{
			GetInstance()->meshRenderers.erase(GetInstance()->meshRenderers.begin() + i);
			break;
		}
	}
	SortMeshRenderers();
}

void RenderingManager::AddPostProcessMaterial(const std::shared_ptr<PostProcessMaterial>& ppMaterial)
{
	GetInstance()->postProcessMaterials.push_back(ppMaterial);
}

void RenderingManager::RemovePostProcessMaterial(const std::shared_ptr<PostProcessMaterial>& ppMaterial)
{
	for (size_t i = 0; i < GetInstance()->postProcessMaterials.size(); i++)
	{
		if (ppMaterial == GetInstance()->postProcessMaterials[i])
		{
			GetInstance()->postProcessMaterials.erase(GetInstance()->postProcessMaterials.begin() + i);
			break;
		}
	}
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

void RenderingManager::SortPostProcessMaterials()
{
	std::sort(GetInstance()->postProcessMaterials.begin(), GetInstance()->postProcessMaterials.end(), ComparePostProcessMaterialsPositions);
	GetInstance()->usedPostProcessMaterials.clear();
	for (size_t i = 0; i < GetInstance()->postProcessMaterials.size(); i++)
	{
		if (!Math::IsNearlyZero(GetInstance()->postProcessMaterials[i]->GetBlendWeight()))
		{
			GetInstance()->usedPostProcessMaterials.push_back(GetInstance()->postProcessMaterials[i]);
		}
	}
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

void RenderingManager::SetSelectedGameObject(GameObject* selectedObject)
{
	GetInstance()->selectedGameObject = selectedObject;
}

void RenderingManager::EnablePostProcessing(bool enable)
{
	GetInstance()->postProcessingEnabled = enable;
}

bool RenderingManager::IsPostProcessingEnabled()
{
	return (GetInstance()->postProcessingEnabled);
}

void RenderingManager::EnableBloom(bool enable)
{
	GetInstance()->bloomEnabled = enable;
}

bool RenderingManager::IsBloomEnabled()
{
	return (GetInstance()->bloomEnabled);
}

void RenderingManager::EnableHDRToneMappingAndGamma(bool enable)
{
	GetInstance()->hdrTonemappingAndGamma = enable;
}

bool RenderingManager::IsHDRToneMappingAndGammaEnabled()
{
	return (GetInstance()->hdrTonemappingAndGamma);
}

void RenderingManager::SetWireframeOnly(bool wireframeOnly)
{
	glPolygonMode(GL_FRONT_AND_BACK, wireframeOnly ? GL_LINE : GL_FILL);
	EnablePostProcessing(!wireframeOnly);
	GetInstance()->renderWireframeOnly = wireframeOnly;
}

bool RenderingManager::IsWireframeOnly()
{
	return GetInstance()->renderWireframeOnly;
}

void RenderingManager::EnableNormalsDebug(bool enable)
{
	GetInstance()->normalsDebugEnabled = enable;
}

bool RenderingManager::IsNormalsDebugEnabled()
{
	return GetInstance()->normalsDebugEnabled;
}

void RenderingManager::SetGamma(float gammaVal)
{
	GetInstance()->SetGammaInternal(gammaVal);
}

float RenderingManager::GetGamma()
{
	return GetInstance()->gamma;
}

void RenderingManager::SetGammaInternal(float gammaVal)
{
	gamma = gammaVal;
	if (hdrToneMappingGammaCorrectionMaterial != nullptr)
	{
		hdrToneMappingGammaCorrectionMaterial->SetFloat("gamma", gamma);
	}
}

void RenderingManager::SetExposure(float exposureVal)
{
	GetInstance()->SetExposureInternal(exposureVal);
}

float RenderingManager::GetExposure()
{
	return GetInstance()->exposure;
}

void RenderingManager::SetExposureInternal(float exposureVal)
{
	exposure = exposureVal;
	if (hdrToneMappingGammaCorrectionMaterial != nullptr)
	{
		hdrToneMappingGammaCorrectionMaterial->SetFloat("exposure", exposure);
	}
}

void RenderingManager::SetBloomBlurAmount(int amount)
{
	GetInstance()->bloomBlurAmount = amount;
}

int RenderingManager::GetBloomBlurAmount()
{
	return GetInstance()->bloomBlurAmount;
}

void RenderingManager::SetShadowMapResolution(unsigned shadowMapRes)
{
	GetInstance()->SetShadowMapResolutionInternal(shadowMapRes);
}

unsigned int RenderingManager::GetShadowMapResolution()
{
	// shadow maps are currently square textures
	return GetInstance()->shadowWidth;
}

void RenderingManager::SetShadowMapResolutionInternal(unsigned shadowMapRes)
{
	glDeleteTextures(1, &directionalDepthMap);
	glDeleteTextures(1, &spotLightDepthMap);
	glDeleteTextures(1, &omniDepthMap);

	shadowWidth = shadowMapRes;
	shadowHeight = shadowMapRes;

	ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, directionalDepthMapFBO, directionalDepthMap, false);
	ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, spotLightDepthMapFBO, spotLightDepthMap, false);
	ConfigureShadowCubeMapFramebuffer(shadowWidth, shadowHeight, omniDepthMapFBO, omniDepthMap, false);
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

bool RenderingManager::ComparePostProcessMaterialsPositions(const std::shared_ptr<PostProcessMaterial>& ppm1,
	const std::shared_ptr<PostProcessMaterial>& ppm2)
{
	return (ppm1->GetPostProcessOrder() < ppm2->GetPostProcessOrder());
}