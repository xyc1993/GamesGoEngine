#include "Component.h"

namespace GamesGoEngine
{
	void Component::Init(GameObject* owner)
	{
		this->owner = owner;
	}

	GameObject* Component::GetOwner() const
	{
		return owner;
	}

	bool Component::IsValid(const Component* component)
	{
		if (component == nullptr)
		{
			return false;
		}

		if (component->owner == nullptr)
		{
			return false;
		}

		return true;
	}
}