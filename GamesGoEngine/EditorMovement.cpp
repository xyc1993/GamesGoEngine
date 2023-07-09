#include "EditorMovement.h"

#include "GameObject.h"
#include "InputManager.h"
#include "Time.h"

EditorMovement::EditorMovement()
{
	movementSpeed = 6.0f;
	rotationSpeed = 0.2f;
	constrainPitch = true;
}

void EditorMovement::Init(GameObject* owner)
{
	Component::Init(owner);

	if (owner != nullptr)
	{
		currentRotation = owner->GetTransform()->GetRotationEulerDegrees();

		// we assume that roll is not supported for basic editor movement
		// because of that we need to recalculate euler angles if roll is 180 or -180 degrees (can happen)
		if (currentRotation.z > 90.0f)
		{
			currentRotation.x -= 180.0f;
			currentRotation.y -= 180.0f;
			currentRotation.z = 0.0f;
		}

		if (currentRotation.z < -90.0f)
		{
			currentRotation.x += 180.0f;
			currentRotation.y += 180.0f;
			currentRotation.z = 0.0f;
		}
	}
}

void EditorMovement::Update()
{
	if (owner != nullptr)
	{
		const float deltaTime = (float)Time::GetUnscaledDeltaTime();
		UpdateRotation(deltaTime);		
		UpdatePosition(deltaTime);
	}
}

void EditorMovement::UpdatePosition(const float deltaTime) const
{
	if (InputManager::GetCursorEnabled())
	{
		return;
	}

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

void EditorMovement::UpdateRotation(const float deltaTime)
{
	// for consistent movement in different frame rates DON'T multiply by deltaTime as it is already covered by GetMouseInput()
	const GLfloat pitch = InputManager::GetMouseYInput() * rotationSpeed;// * deltaTime;
	const GLfloat yaw = InputManager::GetMouseXInput() * rotationSpeed;// * deltaTime;
	
	currentRotation.x -= pitch;
	currentRotation.y -= yaw;
	
	if (constrainPitch)
	{
		if (currentRotation.x > 89.0f)
		{
			currentRotation.x = 89.0f;
		}

		if (currentRotation.x < -89.0f)
		{
			currentRotation.x = -89.0f;
		}
	}

	owner->GetTransform()->SetRotationEulerDegrees(currentRotation);
}