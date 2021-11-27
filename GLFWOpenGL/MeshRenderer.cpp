#include "MeshRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"

// not a good way to handle this (especially without encapsulation), but 'projection' and 'view' should be accessible to all renderers so for now it's fine
extern glm::mat4 view_global;
extern glm::mat4 projection_global;

MeshRenderer::MeshRenderer()
{
	material = nullptr;
}

MeshRenderer::~MeshRenderer()
{
	material->DeleteSafely();
	mesh->DeleteSafely();
}

void MeshRenderer::Init(GameObject* owner)
{
	Component::Init(owner);
	
}

void MeshRenderer::Update()
{
	if (owner != nullptr && material != nullptr && mesh != nullptr)
	{
		material->Draw(owner->GetTransform()->GetModelMatrix(), view_global, projection_global);		
		mesh->Draw();
	}
}

void MeshRenderer::SetMaterial(Material* material)
{
	// firstly, if there's some material assigned, decrement its users number
	if (this->material != nullptr)
	{
		this->material->DecrementNumberOfUsers();
	}

	// secondly assign new material & increment the number of users for the new material
	this->material = material;
	this->material->IncrementNumberOfUsers();
}

void MeshRenderer::SetMesh(MeshBase* mesh)
{
	// firstly, if there's some mesh assigned, decrement its users number
	if (this->mesh != nullptr)
	{
		this->mesh->DecrementNumberOfUsers();
	}

	// secondly assign new mesh & increment the number of users for the new mesh
	this->mesh = mesh;
	this->mesh->IncrementNumberOfUsers();
}