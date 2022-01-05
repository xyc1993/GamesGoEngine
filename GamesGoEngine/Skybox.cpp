#include "Skybox.h"

#include "CamerasManager.h"
#include "MeshPrimitiveCube.h"

Skybox::Skybox()
{
	mesh = new MeshPrimitiveCube();
}

Skybox::~Skybox()
{
	if (material != nullptr)
	{
		material->DeleteSafely();
	}
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

void Skybox::SetMaterial(Material* material)
{
	// firstly, if there's some material assigned, decrement its users number
	if (this->material != nullptr)
	{
		this->material->DecrementNumberOfUsers();
	}

	// secondly assign new material at given index & increment the number of users for the new material
	this->material = material;
	this->material->IncrementNumberOfUsers();
}