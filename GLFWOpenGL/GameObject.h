#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "Component.h"

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
};

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void AddComponent(Component* component);

	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);

private:
	Transform transform;
	std::vector<Component*> components;
};