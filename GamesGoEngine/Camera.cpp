#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "CamerasManager.h"
#include "GameObject.h"

namespace GamesGoEngine
{
	Camera::Camera()
	{
		currentViewMatrix = glm::mat4(1.0f);
		priority = 0;
		fov = 45.0f;
		nearClipPlane = 0.1f;
		farClipPlane = 1000.0f;
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
		CalculateViewMatrix();
		CalculateProjectionMatrix();
	}

	void Camera::CalculateViewMatrix()
	{
		if (owner != nullptr)
		{
			currentViewMatrix = glm::lookAt(owner->GetTransform()->GetPosition(),
				owner->GetTransform()->GetPosition() + owner->GetTransform()->GetForward(),
				owner->GetTransform()->GetUp());
		}
		else
		{
			currentViewMatrix = glm::mat4(1.0f);
		}
	}

	void Camera::CalculateProjectionMatrix()
	{
		currentProjectionMatrix = glm::perspective(glm::radians(fov), CamerasManager::GetCurrentViewAspectRatio(), nearClipPlane, farClipPlane);
	}

	void Camera::SetCameraPriority(int priority)
	{
		this->priority = priority;
	}

	void Camera::SetFoV(float fov)
	{
		this->fov = fov;
	}

	void Camera::SetNearClipPlane(float nearClipPlane)
	{
		this->nearClipPlane = nearClipPlane;
	}

	void Camera::SetFarClipPlane(float farClipPlane)
	{
		this->farClipPlane = farClipPlane;
	}

	glm::mat4 Camera::GetCurrentViewMatrix() const
	{
		return currentViewMatrix;
	}

	glm::mat4 Camera::GetCurrentProjectionMatrix() const
	{
		return currentProjectionMatrix;
	}

	int Camera::GetCameraPriority() const
	{
		return priority;
	}

	float Camera::GetFoV() const
	{
		return fov;
	}

	float Camera::GetNearClipPlane() const
	{
		return nearClipPlane;
	}

	float Camera::GetFarClipPlane() const
	{
		return farClipPlane;
	}

	void Camera::InitMetaData()
	{
		metaData.className = CLASS_NAME(Camera);

		ADD_FIELD_META_DATA(metaData, int, priority, this->priority)

		ADD_FIELD_META_DATA(metaData, float, fov, this->fov)
		ADD_FIELD_META_DATA(metaData, float, nearClipPlane, this->nearClipPlane)
		ADD_FIELD_META_DATA(metaData, float, farClipPlane, this->farClipPlane)
	}
}