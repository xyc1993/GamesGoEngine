#pragma once

#include <set>
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

	void SetParent(GameObject* parent);

private:
	void RemoveParent();
	void SetNewParent(GameObject* parent);

	std::string name;
	Transform* transform;
	std::vector<Component*> components;

	GameObject* parent;
	// direct children of the game object
	std::vector<GameObject*> children;
	// all of the children won the game object hierarchy (i.e. children of children as well)
	std::set<GameObject*> allChildren;
};