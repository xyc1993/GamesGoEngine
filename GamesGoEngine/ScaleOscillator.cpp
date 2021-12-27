#include "ScaleOscillator.h"

#include "GameObject.h"

extern float currentTime;

void ScaleOscillator::Init(GameObject* owner)
{
	Component::Init(owner);

	if (owner != nullptr && owner->GetTransform() != nullptr)
	{
		startScale = owner->GetTransform()->GetScale();
	}
}

void ScaleOscillator::Update()
{
	if (owner != nullptr)
	{
		const glm::vec3 scale = startScale + amplitude * sin(speed * currentTime);
		owner->GetTransform()->SetScale(scale);
	}
}

void ScaleOscillator::SetAmplitude(glm::vec3 amplitude)
{
	this->amplitude = amplitude;
}

void ScaleOscillator::SetSpeed(float speed)
{
	this->speed = speed;
}