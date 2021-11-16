#pragma once

#include <vector>

#include "Transform.h"
#include "Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void AddComponent(Component* component);

	Transform* GetTransform() const;

private:
	Transform* transform;
	std::vector<Component*> components;
};