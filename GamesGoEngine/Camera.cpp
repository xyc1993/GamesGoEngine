#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"

extern glm::mat4 view_global;

void Camera::Update()
{
	view_global = GetViewMatrix();
}

glm::mat4 Camera::GetViewMatrix() const
{
	if (owner != nullptr)
	{
		return glm::lookAt(owner->GetTransform()->GetPosition(),
			owner->GetTransform()->GetPosition() + owner->GetTransform()->GetForward(),
			owner->GetTransform()->GetUp());
	}
	else
	{
		return glm::mat4(1.0f);
	}
}