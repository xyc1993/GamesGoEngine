#include "Rotator.h"

#include "GameObject.h"

extern float deltaTime;

void Rotator::Update()
{
	if (owner != nullptr && owner->GetTransform() != nullptr)
	{
		const glm::quat frameRotation = glm::quat(glm::radians(speed) * deltaTime);
		owner->GetTransform()->SetRotation(frameRotation * owner->GetTransform()->GetRotation());
	}
}

void Rotator::SetSpeed(glm::vec3 speed)
{
	this->speed = speed;
}