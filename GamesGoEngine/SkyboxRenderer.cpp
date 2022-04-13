#include "SkyboxRenderer.h"

#include "CamerasManager.h"
#include "MeshPrimitiveCube.h"
#include "RenderingManager.h"

SkyboxRenderer::SkyboxRenderer()
{
	mesh = std::make_shared<MeshPrimitiveCube>();
	materialList.clear();
	materialList.resize(mesh->GetSubMeshesCount());
	RenderingManager::SetSkybox(this);
}

void SkyboxRenderer::Draw()
{
	glDisable(GL_STENCIL_TEST);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_FRONT);

	Renderer::Draw();

	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
}

void SkyboxRenderer::SetMaterial(const std::shared_ptr<Material>& material)
{
	if (materialList[0] != material)
	{
		materialList[0].reset();
		materialList[0] = material;
	}
}

glm::mat4 SkyboxRenderer::GetViewMatrix() const
{
	return glm::mat4(glm::mat3(CamerasManager::GetActiveCameraViewMatrix()));
}