#include "RenderingManager.h"

#include <algorithm>

#include "CamerasManager.h"
#include "GameObject.h"
#include "MeshPrimitiveQuad.h"
#include "MeshRenderer.h"
#include "PostProcessMaterial.h"
#include "PostProcessRenderer.h"

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
	
	// set the outline materials
	editorOutlineMaterialScale = new Material("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");
	editorOutlineMaterialNormals = new Material("res/shaders/unlitProtruded.vert.glsl", "res/shaders/unlit.frag.glsl");
	
	constexpr glm::vec3 editorOutlineColor = glm::vec3(1.0f, 0.8f, 0.0f);
	editorOutlineMaterialScale->SetVector3((GLchar*)"unlitColor", editorOutlineColor);
	editorOutlineMaterialNormals->SetVector3((GLchar*)"unlitColor", editorOutlineColor);

	constexpr float editorOutlineNormalsProtrusion = 0.03f;
	editorOutlineMaterialNormals->SetFloat((GLchar*)"protrusion", editorOutlineNormalsProtrusion);
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

void RenderingManager::Update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer1);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glEnable(GL_STENCIL_TEST);

	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glStencilMask(~0);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);	

	// TODO: more optimal sorting, it could sort on camera view change, not on every draw frame
	SortTransparentMeshRenderers();

	DrawSkybox();
	DrawRenderers(GetInstance()->opaqueMeshRenderers);
	DrawRenderers(GetInstance()->transparentMeshRenderers);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	
	for (size_t i = 0; i < GetInstance()->postProcessRenderers.size(); i++)
	{
		if (i == (GetInstance()->postProcessRenderers.size() - 1))
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
		if (GetInstance()->postProcessRenderers[i]->TryGetMaterial(ppMaterial, 0))
		{
			ppMaterial->SetTexture("screenTexture", 0, i % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);
			ppMaterial->SetTexture("depthStencilTexture", 1, i % 2 == 0 ? depthStencilBuffer1 : depthStencilBuffer2);
			ppMaterial->SetTexture("stencilView", 2, i % 2 == 0 ? stencilView1 : stencilView2);
			GetInstance()->postProcessRenderers[i]->Draw();
		}
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

Material* RenderingManager::GetEditorOutlineMaterial(bool isMeshImported)
{
	if (isMeshImported)
	{
		return GetInstance()->editorOutlineMaterialNormals;
	}
	else
	{
		return GetInstance()->editorOutlineMaterialScale;
	}
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