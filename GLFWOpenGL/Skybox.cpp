#include "Skybox.h"

#include "MeshPrimitiveCube.h"

// not a good way to handle this (especially without encapsulation), but 'projection' and 'view' should be accessible to all renderers so for now it's fine
extern glm::mat4 view_global;
extern glm::mat4 projection_global;

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

		// for now let's assume there's only 1 mesh (cube) & 1 material slot (cube mesh material), it's enough for this implementation of the skybox
		material->Draw(glm::mat4(1.0f), glm::mat4(glm::mat3(view_global)), projection_global);
		mesh->DrawSubMesh();

		glDepthFunc(GL_LESS);
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