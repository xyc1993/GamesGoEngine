#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "CamerasManager.h"
#include "GameObject.h"

Camera::Camera()
{
	currentViewMatrix = glm::mat4(1.0f);
	priority = 0;
}

Camera::~Camera()
{
	CamerasManager::RemoveSceneCamera(this);
}

void Camera::Init(GameObject* owner)
{
	Component::Init(owner);
	CamerasManager::AddSceneCamera(this);
}

void Camera::Update()
{
	currentViewMatrix = GetViewMatrix();
}

void Camera::SetCameraPriority(int priority)
{
	this->priority = priority;
}

glm::mat4 Camera::GetCurrentViewMatrix() const
{
	return currentViewMatrix;
}

int Camera::GetCameraPriority() const
{
	return priority;
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
