#include "PositionOscillator.h"

#include "GameObject.h"

extern float currentTime;

void PositionOscillator::Init(GameObject* owner)
{
	Component::Init(owner);

	if (owner != nullptr && owner->GetTransform() != nullptr)
	{
		startPosition = owner->GetTransform()->GetPosition();
	}	
}

void PositionOscillator::Update()
{
	if (owner != nullptr)
	{
		const glm::vec3 position = startPosition + amplitude * sin(speed * currentTime);
		owner->GetTransform()->SetPosition(position);
	}
}

void PositionOscillator::SetAmplitude(glm::vec3 amplitude)
{
	this->amplitude = amplitude;
}

void PositionOscillator::SetSpeed(float speed)
{
	this->speed = speed;
}