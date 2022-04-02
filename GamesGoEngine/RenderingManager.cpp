#include "RenderingManager.h"

#include <algorithm>

#include "CamerasManager.h"
#include "GameObject.h"
#include "MeshPrimitiveQuad.h"
#include "MeshRenderer.h"
#include "PostProcessMaterial.h"
#include "PostProcessRenderer.h"

RenderingManager* RenderingManager::instance = nullptr;
unsigned int RenderingManager::framebuffer = -1;
unsigned int RenderingManager::framebuffer2 = -1;
unsigned int RenderingManager::textureColorbuffer = -1;
unsigned int RenderingManager::textureColorbuffer2 = -1;
unsigned int RenderingManager::rbo = -1;
unsigned int RenderingManager::rbo2 = -1;

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

	std::shared_ptr<MeshPrimitiveQuad> quadMesh = std::make_shared<MeshPrimitiveQuad>();
	screenMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/screenShader.frag.glsl");
	screenRenderer = new PostProcessRenderer();
	screenRenderer->SetMaterial(screenMaterial);
	GameObject* screenRendererObject = new GameObject();
	screenRendererObject->SetName("screen_renderer_object");
	screenRendererObject->AddComponent(screenRenderer);
	
	screenMaterialTest = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/screenShader.frag.glsl");
	screenRendererTest = new PostProcessRenderer();
	screenRendererTest->SetMaterial(screenMaterialTest);
	GameObject* screenRendererObjectTest = new GameObject();
	screenRendererObjectTest->SetName("screen_renderer_object");
	screenRendererObjectTest->AddComponent(screenRendererTest);

	screenMaterial2 = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/inverseColor.frag.glsl");
	screenRenderer2 = new PostProcessRenderer();
	screenRenderer2->SetMaterial(screenMaterial2);
	GameObject* screenRendererObject2 = new GameObject();
	screenRendererObject2->SetName("screen_renderer_object_test");
	screenRendererObject2->AddComponent(screenRenderer2);

	screenMaterial3 = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/blur.frag.glsl");
	screenRenderer3 = new PostProcessRenderer();
	screenRenderer3->SetMaterial(screenMaterial3);
	GameObject* screenRendererObject3 = new GameObject();
	screenRendererObject3->SetName("screen_renderer_object_test2");
	screenRendererObject3->AddComponent(screenRenderer3);
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
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glGenFramebuffers(1, &framebuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

	// create a color attachment texture
	glGenTextures(1, &textureColorbuffer2);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo2);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo2); // now actually attach it

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GetInstance()->screenMaterial->SetTexture("screenTexture", textureColorbuffer2);
	GetInstance()->screenMaterialTest->SetTexture("screenTexture", textureColorbuffer);

	GetInstance()->screenMaterial2->SetTexture("screenTexture", textureColorbuffer);
	GetInstance()->screenMaterial3->SetTexture("screenTexture", textureColorbuffer2);
}

void RenderingManager::ResizeBuffers(GLint screenWidth, GLint screenHeight)
{
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

	glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
}

void RenderingManager::Update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	

	// TODO: more optimal sorting, it could sort on camera view change, not on every draw frame
	SortTransparentMeshRenderers();

	DrawSkybox();
	DrawRenderers(GetInstance()->opaqueMeshRenderers);
	DrawRenderers(GetInstance()->transparentMeshRenderers);

	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
	// disable depth test so screen-space quad isn't discarded due to depth test.
	glDisable(GL_DEPTH_TEST);
	// set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GetInstance()->screenRenderer2->Draw();

	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// disable depth test so screen-space quad isn't discarded due to depth test.
	glDisable(GL_DEPTH_TEST);
	// set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GetInstance()->screenRenderer3->Draw();
	
	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// disable depth test so screen-space quad isn't discarded due to depth test.
	glDisable(GL_DEPTH_TEST);
	// set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GetInstance()->screenRendererTest->Draw();
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

void RenderingManager::SetSkybox(Skybox* skybox)
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