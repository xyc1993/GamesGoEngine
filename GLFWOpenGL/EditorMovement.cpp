#include "EditorMovement.h"

#include "GameObject.h"
#include "InputManager.h"

extern GLfloat deltaTime;

EditorMovement::EditorMovement()
{
	movementSpeed = 6.0f;
	rotationSpeed = 0.25f;
	constrainPitch = true;
}

void EditorMovement::Update()
{
	if (owner != nullptr)
	{
		UpdatePosition();
		UpdateRotation();		
	}
}

void EditorMovement::UpdatePosition() const
{
	if (InputManager::GetKey(GLFW_KEY_W) || InputManager::GetKey(GLFW_KEY_UP))
	{
		const glm::vec3 newPosition = owner->GetTransform()->GetPosition() + movementSpeed * owner->GetTransform()->GetForward() * deltaTime;
		owner->GetTransform()->SetPosition(newPosition);
	}

	if (InputManager::GetKey(GLFW_KEY_S) || InputManager::GetKey(GLFW_KEY_DOWN))
	{
		const glm::vec3 newPosition = owner->GetTransform()->GetPosition() - movementSpeed * owner->GetTransform()->GetForward() * deltaTime;
		owner->GetTransform()->SetPosition(newPosition);
	}

	if (InputManager::GetKey(GLFW_KEY_D) || InputManager::GetKey(GLFW_KEY_RIGHT))
	{
		const glm::vec3 newPosition = owner->GetTransform()->GetPosition() + movementSpeed * owner->GetTransform()->GetRight() * deltaTime;
		owner->GetTransform()->SetPosition(newPosition);
	}

	if (InputManager::GetKey(GLFW_KEY_A) || InputManager::GetKey(GLFW_KEY_LEFT))
	{
		const glm::vec3 newPosition = owner->GetTransform()->GetPosition() - movementSpeed * owner->GetTransform()->GetRight() * deltaTime;
		owner->GetTransform()->SetPosition(newPosition);
	}

	if (InputManager::GetKey(GLFW_KEY_E))
	{
		const glm::vec3 newPosition = owner->GetTransform()->GetPosition() + movementSpeed * owner->GetTransform()->GetUp() * deltaTime;
		owner->GetTransform()->SetPosition(newPosition);
	}

	if (InputManager::GetKey(GLFW_KEY_Q))
	{
		const glm::vec3 newPosition = owner->GetTransform()->GetPosition() - movementSpeed * owner->GetTransform()->GetUp() * deltaTime;
		owner->GetTransform()->SetPosition(newPosition);
	}
}

void EditorMovement::UpdateRotation() const
{
	const GLfloat pitch = InputManager::GetMouseYInput() * rotationSpeed * deltaTime;
	const GLfloat yaw = InputManager::GetMouseXInput() * rotationSpeed * deltaTime;

	glm::vec3 newRotation = owner->GetTransform()->GetRotation();
	newRotation.x -= pitch;
	newRotation.y -= yaw;

	if (constrainPitch)
	{
		if (newRotation.x > 89.0f)
		{
			newRotation.x = 89.0f;
		}

		if (newRotation.x < -89.0f)
		{
			newRotation.x = -89.0f;
		}
	}

	owner->GetTransform()->SetRotation(newRotation);
}