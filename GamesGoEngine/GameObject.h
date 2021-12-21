#pragma once

#include <string>
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
	std::string GetName() const;
	void SetName(std::string name);

private:
	std::string name;
	Transform* transform;
	std::vector<Component*> components;
};