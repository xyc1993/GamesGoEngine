#include "Skybox.h"

#include "CamerasManager.h"
#include "MeshPrimitiveCube.h"

Skybox::Skybox()
{
	mesh = new MeshPrimitiveCube();
}

Skybox::~Skybox()
{
	this->material.reset();
	delete mesh;
}

void Skybox::Update()
{
	if (material != nullptr && mesh != nullptr)
	{
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_FRONT);

		// for now let's assume there's only 1 mesh (cube) & 1 material slot (cube mesh material), it's enough for this implementation of the skybox
		material->Draw(glm::mat4(1.0f), glm::mat4(glm::mat3(CamerasManager::GetActiveCameraViewMatrix())), CamerasManager::GetActiveCameraProjectionMatrix());
		mesh->DrawSubMesh();

		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);
	}
}

void Skybox::SetMaterial(const std::shared_ptr<Material>& material)
{
	if (this->material != material)
	{
		this->material.reset();
		this->material = material;
	}
}