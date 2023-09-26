#include "ScaleOscillator.h"

#include "GameObject.h"
#include "Time.h"

namespace GamesGoEngine
{
	void ScaleOscillator::Init(GameObject* owner)
	{
		Component::Init(owner);

		if (owner != nullptr && owner->GetTransform() != nullptr)
		{
			startScale = owner->GetTransform()->GetLocalScale();
		}
	}

	void ScaleOscillator::Update()
	{
		if (owner != nullptr)
		{
			const glm::vec3 scale = startScale + amplitude * sin(speed * (float)Time::GetTime());
			owner->GetTransform()->SetLocalScale(scale);
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

	void ScaleOscillator::InitMetaData()
	{
		metaData.className = CLASS_NAME(ScaleOscillator);
		
		ADD_FIELD_META_DATA(metaData, glm::vec3, amplitude, this->amplitude)
		ADD_FIELD_META_DATA(metaData, float, speed, this->speed)
	}
}