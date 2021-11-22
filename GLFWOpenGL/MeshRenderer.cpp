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
	if (material != nullptr)
	{
		material->~Material();		
	}
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
	this->material = material;
}

void MeshRenderer::SetMesh(MeshBase* mesh)
{
	this->mesh = mesh;
}