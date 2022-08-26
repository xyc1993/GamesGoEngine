#include "RenderingManager.h"

#include <algorithm>

#include "CamerasManager.h"
#include "GameObject.h"
#include "Math.h"
#include "MeshPrimitiveQuad.h"
#include "MeshRenderer.h"
#include "PostProcessMaterial.h"
#include "PostProcessRenderer.h"
#include "Time.h"

RenderingManager* RenderingManager::instance = nullptr;
unsigned int RenderingManager::framebuffer1 = -1;
unsigned int RenderingManager::framebuffer2 = -1;
unsigned int RenderingManager::textureColorBuffer1 = -1;
unsigned int RenderingManager::textureColorBuffer2 = -1;
unsigned int RenderingManager::depthStencilBuffer1 = -1;
unsigned int RenderingManager::depthStencilBuffer2 = -1;
unsigned int RenderingManager::stencilView1 = -1;
unsigned int RenderingManager::stencilView2 = -1;

RenderingManager::RenderingManager()
{
	lightsManager = new LightsManager();
	normalDebugMaterial = new Material("res/shaders/debugNormals.vert.glsl", "res/shaders/debugNormals.frag.glsl", "res/shaders/debugNormals.geom.glsl");
}

RenderingManager::~RenderingManager()
{
	delete lightsManager;
	delete normalDebugMaterial;
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

	// framebuffer configuration
	ConfigureFramebuffer(screenWidth, screenHeight, framebuffer1, textureColorBuffer1, depthStencilBuffer1, stencilView1);
	ConfigureFramebuffer(screenWidth, screenHeight, framebuffer2, textureColorBuffer2, depthStencilBuffer2, stencilView2);

	// configure uniform buffer objects
	GetInstance()->ConfigureUniformBufferObjects();

	// debug materials
	GetInstance()->CreateDebugMaterials();
}

void RenderingManager::ConfigureFramebuffer(GLint screenWidth, GLint screenHeight, unsigned int& framebuffer, unsigned int& textureColorBuffer, unsigned int& depthStencilBuffer, unsigned int& stencilView)
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

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

void RenderingManager::ResizeBuffers(GLint screenWidth, GLint screenHeight)
{
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, depthStencilBuffer1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH24_STENCIL8, GL_UNSIGNED_BYTE, NULL);
	
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, depthStencilBuffer2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH24_STENCIL8, GL_UNSIGNED_BYTE, NULL);
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
	if (IsPostProcessingEnabled())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer1);
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

	// update uniform buffer objects
	GetInstance()->UpdateUniformBufferObjects();

	// TODO: more optimal sorting, it could sort on camera view change, not on every draw frame
	SortTransparentMeshRenderers();

	DrawRenderers(GetInstance()->opaqueMeshRenderers);
	DrawRenderers(GetInstance()->transparentMeshRenderers);
	DrawSkybox();

	if (IsNormalsDebugEnabled() && (GetInstance()->normalDebugMaterial != nullptr))
	{
		DrawRenderers(GetInstance()->meshRenderers, GetInstance()->normalDebugMaterial);
	}

	if (GetInstance()->firstRenderedFrame)
	{
		GetInstance()->firstRenderedFrame = false;
		SortPostProcessRenderers();
	}

	if (IsPostProcessingEnabled())
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		
		for (size_t i = 0; i < GetInstance()->usedPostProcessRenderers.size(); i++)
		{
			if (i == (GetInstance()->usedPostProcessRenderers.size() - 1))
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, i % 2 == 0 ? framebuffer2 : framebuffer1);
			}

			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			std::shared_ptr<Material> ppMaterial;
			if (GetInstance()->usedPostProcessRenderers[i]->TryGetMaterial(ppMaterial, 0))
			{
				ppMaterial->SetTexture("screenTexture", 0, i % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);
				ppMaterial->SetTexture("depthStencilTexture", 1, i % 2 == 0 ? depthStencilBuffer1 : depthStencilBuffer2);
				ppMaterial->SetTexture("stencilView", 2, i % 2 == 0 ? stencilView1 : stencilView2);
				GetInstance()->usedPostProcessRenderers[i]->Draw();
			}
		}
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

void RenderingManager::AddPostProcessRenderer(PostProcessRenderer* postProcessRenderer)
{
	GetInstance()->postProcessRenderers.push_back(postProcessRenderer);
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

void RenderingManager::SortPostProcessRenderers()
{
	std::sort(GetInstance()->postProcessRenderers.begin(), GetInstance()->postProcessRenderers.end(), ComparePostProcessRenderersPositions);
	GetInstance()->usedPostProcessRenderers.clear();
	for (size_t i = 0; i < GetInstance()->postProcessRenderers.size(); i++)
	{
		if (!Math::IsNearlyZero(GetInstance()->postProcessRenderers[i]->GetBlendWeight()))
		{
			GetInstance()->usedPostProcessRenderers.push_back(GetInstance()->postProcessRenderers[i]);
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

void RenderingManager::EnablePostProcessing(bool enable)
{
	GetInstance()->postProcessingEnabled = enable;
}

bool RenderingManager::IsPostProcessingEnabled()
{
	return (GetInstance()->postProcessingEnabled && !GetInstance()->usedPostProcessRenderers.empty());
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

bool RenderingManager::ComparePostProcessRenderersPositions(PostProcessRenderer* ppr1, PostProcessRenderer* ppr2)
{
	return (ppr1->GetPostProcessOrder() < ppr2->GetPostProcessOrder());
}