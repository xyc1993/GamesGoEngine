#pragma once

#include "ReflectableObject.h"

namespace GamesGoEngine
{
	class GameObject;

	class Component : public ReflectableObject
	{
	public:
		virtual ~Component() = default;
		virtual void Init(GameObject* owner);
		virtual void Update() = 0;
		virtual void LateUpdate() {};
		virtual void OnSelected() {};
		virtual void OnDeselected() {};
		GameObject* GetOwner() const;
		static bool IsValid(const Component* component);

	protected:
		GameObject* owner;
	};
}