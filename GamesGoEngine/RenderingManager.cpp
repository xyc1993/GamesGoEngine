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
	normalDebugMaterial = new Material("res/shaders/debugNormals.vert.glsl", "res/shaders/debugNormals.frag.glsl", "res/shaders/debugNormals.geom.glsl");
	brightPixelsExtractionMaterial = std::make_shared<PostProcessMaterial>("res/shaders/RenderPipeline/extractBrightColor.frag.glsl");
	bloomBlurMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/blurGaussian.frag.glsl");
	hdrToneMappingGammaCorrectionMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/hdrToneMappingGammaCorrection.frag.glsl");
	editorOutlineMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/editorOutline.frag.glsl");
	depthMapMaterial = new Material("res/shaders/RenderPipeline/depthMap.vert.glsl", "res/shaders/RenderPipeline/depthMap.frag.glsl");
	omniDepthMapMaterial = new Material("res/shaders/RenderPipeline/omniDepthMap.vert.glsl", "res/shaders/RenderPipeline/omniDepthMap.frag.glsl", "res/shaders/RenderPipeline/omniDepthMap.geom.glsl");
	orientationDebugMaterial = new Material("res/shaders/DebugShaders/debugOrientation.vert.glsl", "res/shaders/DebugShaders/debugOrientation.frag.glsl", "res/shaders/DebugShaders/debugOrientation.geom.glsl");	
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
	GetInstance()->InitEditorOutline();
}

void RenderingManager::InitGammaCorrection()
{
	// make sure that the default gamma value & exposure is set in the material
	SetGamma(GetGamma());
	SetExposure(GetExposure());
}

void RenderingManager::InitEditorOutline()
{
	editorOutlineMaterial->SetPostProcessOrder(-1);
	AddPostProcessMaterial(editorOutlineMaterial);
}

void RenderingManager::ConfigureFramebuffers(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer)
{
	ConfigureFramebuffer(screenWidth, screenHeight, framebuffer1, textureColorBuffer1, depthStencilBuffer1, stencilView1, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, framebuffer2, textureColorBuffer2, depthStencilBuffer2, stencilView2, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, bloomFBO1, bloomColorBuffer1, bloomDepthStencilBuffer1, bloomStencilView1, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, bloomFBO2, bloomColorBuffer2, bloomDepthStencilBuffer2, bloomStencilView2, shouldGenerateFramebuffer);
	ConfigureFramebuffer(screenWidth, screenHeight, msFramebuffer, msTextureColorBuffer, msDepthStencilBuffer, msStencilView, shouldGenerateFramebuffer);
	ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, depthMapFBO, depthMap, shouldGenerateFramebuffer);
	ConfigureShadowCubeMapFramebuffer(shadowWidth, shadowHeight, omniDepthMapFBO, omniDepthMap, shouldGenerateFramebuffer);
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

void RenderingManager::ConfigureMultisampledFramebuffer(GLint screenWidth, GLint screenHeight, 
	unsigned& framebuffer, unsigned& textureColorBuffer,
	unsigned& depthStencilBuffer, unsigned& stencilView,
	bool shouldGenerateFramebuffer, int samples)
{
	if (shouldGenerateFramebuffer)
	{
		glGenFramebuffers(1, &framebuffer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F, screenWidth, screenHeight, GL_TRUE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffer, 0);

	// create a depth & stencil attachment texture
	glGenTextures(1, &depthStencilBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthStencilBuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, GL_TRUE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthStencilBuffer, 0);

	glGenTextures(1, &stencilView);
	glTextureView(stencilView, GL_TEXTURE_2D_MULTISAMPLE, depthStencilBuffer, GL_DEPTH24_STENCIL8, 0, 1, 0, 1);

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

void RenderingManager::ResizeBuffers(GLint screenWidth, GLint screenHeight)
{
	GetInstance()->ResizeBuffersInternal(screenWidth, screenHeight);
}

void RenderingManager::ResizeBuffersInternal(GLint screenWidth, GLint screenHeight)
{
	// not the most lightweight approach since we need to recreate textures but it is required due to how we write data from stencil buffer into texture
	glDeleteTextures(1, &textureColorBuffer1);
	glDeleteTextures(1, &textureColorBuffer2);
	glDeleteTextures(1, &bloomColorBuffer1);
	glDeleteTextures(1, &bloomColorBuffer2);
	glDeleteTextures(1, &msTextureColorBuffer);

	glDeleteTextures(1, &depthStencilBuffer1);
	glDeleteTextures(1, &depthStencilBuffer2);
	glDeleteTextures(1, &bloomDepthStencilBuffer1);
	glDeleteTextures(1, &bloomDepthStencilBuffer2);
	glDeleteTextures(1, &msDepthStencilBuffer);

	glDeleteTextures(1, &stencilView1);
	glDeleteTextures(1, &stencilView2);
	glDeleteTextures(1, &bloomStencilView1);
	glDeleteTextures(1, &bloomStencilView2);
	glDeleteTextures(1, &msStencilView);

	glDeleteTextures(1, &depthMap);
	glDeleteTextures(1, &omniDepthMap);

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

	GetInstance()->UpdateShadowMap();
	
	// Rendering the scene
	// reset viewport
	glViewport(0, 0, WindowManager::GetCurrentWidth(), WindowManager::GetCurrentHeight());

	if (IsPostProcessingEnabled())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->msFramebuffer);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glEnable(GL_STENCIL_TEST);

	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glStencilMask(~0);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	DrawRenderers(GetInstance()->opaqueMeshRenderers);
	DrawRenderers(GetInstance()->transparentMeshRenderers);
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

	if (IsPostProcessingEnabled())
	{
		// pass the image from MSAA framebuffer to first framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, GetInstance()->msFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GetInstance()->framebuffer1);

		const int width = WindowManager::GetCurrentWidth();
		const int height = WindowManager::GetCurrentHeight();

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		// post processing via ping pong rendering
		const size_t postProcessEffects = GetInstance()->usedPostProcessMaterials.size();
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

		// extract pixels over bloom's brightness threshold
		int lastPostProcessMaterialIndex = postProcessEffects;
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

		// finally apply hdr tone mapping and gamma correction
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// get texture before bright pixels extraction
		lastPostProcessMaterialIndex--;
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->textureColorBuffer1 : GetInstance()->textureColorBuffer2);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetTexture("depthStencilTexture", 1, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->depthStencilBuffer1 : GetInstance()->depthStencilBuffer2);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetTexture("stencilView", 2, lastPostProcessMaterialIndex % 2 == 0 ? GetInstance()->stencilView1 : GetInstance()->stencilView2);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->SetTexture("bloomBlur", 3, (GetInstance()->bloomBlurAmount - 1) % 2 == 1 ? GetInstance()->bloomColorBuffer1 : GetInstance()->bloomColorBuffer2);
		GetInstance()->hdrToneMappingGammaCorrectionMaterial->Draw(glm::mat4());
		MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
	}
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

void RenderingManager::UpdateShadowMap()
{
	//UpdateDirectionalShadowMap();
	UpdateOmnidirectionalShadowMap();
}

void RenderingManager::UpdateDirectionalShadowMap()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Light* directionalLight = lightsManager->GetDirectionalLight(0);
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
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
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
			outMaterial->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
			outMaterial->SetTexture("shadowMap", 1, depthMap);
			// We want direction from fragment to light position
			outMaterial->SetVector3("lightDir", -directionalLight->GetOwner()->GetTransform()->GetForward());
		}
	}
}

void RenderingManager::UpdateOmnidirectionalShadowMap()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Light* pointLight = lightsManager->GetPointLight(0);
	if (!Component::IsValid(pointLight))
	{
		return;
	}

	float aspect = (float)shadowWidth / (float)shadowHeight;
	float near = 1.0f;
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
			outMaterial->SetCubeTexture("depthMap", 1, omniDepthMap);
			outMaterial->SetFloat("far_plane", far);
			outMaterial->SetVector3("lightPos", lightPos);
		}
	}
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
	return (GetInstance()->postProcessingEnabled && !GetInstance()->usedPostProcessMaterials.empty());
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

void RenderingManager::EnableMSAA(bool enable)
{
	GetInstance()->SetMSAAInternal(enable, GetMSAASamplesNumber());
}

bool RenderingManager::IsMSAAEnabled()
{
	return GetInstance()->msaaEnabled;
}

void RenderingManager::SetMSAASamplesNumber(int samples)
{
	GetInstance()->SetMSAAInternal(IsMSAAEnabled(), samples);
}

int RenderingManager::GetMSAASamplesNumber()
{
	return GetInstance()->msaaSamplesNumber;
}

int RenderingManager::GetMaxMSAASamplesNumber()
{
	int maxSamples;
	glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
	return maxSamples;
}

void RenderingManager::SetMSAAInternal(bool enable, int samples)
{
	glDeleteTextures(1, &msTextureColorBuffer);
	glDeleteTextures(1, &msDepthStencilBuffer);
	glDeleteTextures(1, &msStencilView);

	const GLint width = WindowManager::GetCurrentWidth();
	const GLint height = WindowManager::GetCurrentHeight();

	if (enable)
	{
		ConfigureMultisampledFramebuffer(width, height, msFramebuffer, msTextureColorBuffer, msDepthStencilBuffer, msStencilView, false, samples);
	}
	else
	{
		ConfigureFramebuffer(width, height, msFramebuffer, msTextureColorBuffer, msDepthStencilBuffer, msStencilView, false);
	}

	msaaEnabled = enable;
	msaaSamplesNumber = samples;
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
	glDeleteTextures(1, &depthMap);
	glDeleteTextures(1, &omniDepthMap);

	shadowWidth = shadowMapRes;
	shadowHeight = shadowMapRes;

	ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, depthMapFBO, depthMap, false);
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