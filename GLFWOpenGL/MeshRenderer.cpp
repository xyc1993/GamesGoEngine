#include "MeshRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"

/*
MeshRenderer::~MeshRenderer()
{

}
*/

void MeshRenderer::Init(GameObject* owner)
{
	Component::Init(owner);
	
}

void MeshRenderer::Update()
{
	if (owner != nullptr)
	{
		//calculate model matrix for the mesh
		glm::mat4 model(1.0f);
		model = glm::translate(model, owner->GetTransform().position);
		const glm::quat rotationQuaternion = glm::quat(owner->GetTransform().rotation);
		model = glm::rotate(model, angle(rotationQuaternion), axis(rotationQuaternion));
		model = glm::scale(model, owner->GetTransform().scale);
	}
}