#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "Component.h"

struct Transform
{
	glm::vec3 position{};
	glm::vec3 rotation{}; //rotation expressed in Euler Angles
	glm::vec3 scale{};

	Transform()
	{
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
	}
};

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void AddComponent(Component* component);

	const Transform& GetTransform() const;
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 eulerAngles);
	void SetScale(glm::vec3 scale);

private:
	Transform transform;
	std::vector<Component*> components;
};