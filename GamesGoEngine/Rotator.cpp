#include "Rotator.h"

#include "GameObject.h"
#include "Time.h"

namespace GamesGoEngine
{
	Rotator::Rotator()
	{
		identityQuat = glm::quat(glm::vec3(0.0f));
	}

	void Rotator::Update()
	{
		if (owner != nullptr && owner->GetTransform() != nullptr)
		{
			glm::quat speedQuat = glm::quat(glm::radians(speed));
			const glm::quat frameRotation = glm::mix(identityQuat, speedQuat, (float)Time::GetDeltaTime());
			owner->GetTransform()->SetLocalRotation(frameRotation * owner->GetTransform()->GetLocalRotation());
		}
	}

	void Rotator::SetSpeed(glm::vec3 speed)
	{
		this->speed = speed;
	}

	void Rotator::InitMetaData()
	{
		metaData.className = CLASS_NAME(Rotator);
		
		ADD_FIELD_META_DATA(metaData, glm::vec3, speed, this->speed)
	}
}