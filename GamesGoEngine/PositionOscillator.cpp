#include "PositionOscillator.h"

#include "GameObject.h"
#include "Time.h"

namespace GamesGoEngine
{
	REGISTER_CLASS_STATIC(PositionOscillator);

	void PositionOscillator::Init(GameObject* owner)
	{
		Component::Init(owner);

		if (owner != nullptr && owner->GetTransform() != nullptr)
		{
			startPosition = owner->GetTransform()->GetLocalPosition();
		}
	}

	void PositionOscillator::Update()
	{
		if (owner != nullptr)
		{
			const glm::vec3 position = startPosition + amplitude * sin(speed * (float)Time::GetTime());
			owner->GetTransform()->SetLocalPosition(position);
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

	void PositionOscillator::InitMetaData()
	{
		metaData.className = CLASS_NAME(PositionOscillator);
		
		ADD_FIELD_META_DATA(metaData, glm::vec3, amplitude, this->amplitude)
		ADD_FIELD_META_DATA(metaData, float, speed, this->speed)
	}
}