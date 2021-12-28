#include "Rotator.h"

#include "GameObject.h"

extern float deltaTime;

Rotator::Rotator()
{
	identityQuat = glm::quat(glm::vec3(0.0f));
}

void Rotator::Update()
{
	if (owner != nullptr && owner->GetTransform() != nullptr)
	{
		const glm::quat frameRotation = glm::mix(identityQuat, speed, deltaTime);
		owner->GetTransform()->SetLocalRotation(frameRotation * owner->GetTransform()->GetLocalRotation());
	}
}

void Rotator::SetSpeed(glm::vec3 speed)
{
	this->speed = glm::quat(glm::radians(speed));
}