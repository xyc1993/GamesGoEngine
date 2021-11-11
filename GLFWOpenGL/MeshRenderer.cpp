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
		glBindVertexArray(mesh->GetVAO());

		// calculate model matrix for the mesh
		glm::mat4 model(1.0f);
		model = glm::translate(model, owner->GetTransform().position);
		const glm::quat rotationQuaternion = glm::quat(owner->GetTransform().rotation);
		model = glm::rotate(model, angle(rotationQuaternion), axis(rotationQuaternion));
		model = glm::scale(model, owner->GetTransform().scale);

		material->Draw(model, view_global, projection_global);

		//definitely shouldn't be handled this way, it works only for cube primitive, but for now it's good enough, after some cleanup with cube class it'll be fixed
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
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