#include "GameObject.h"

GameObject::GameObject()
{
	transform = Transform();
	components.clear();
}

GameObject::~GameObject()
{
	for (Component* component : components)
	{
		if (component != nullptr)
		{
			component->~Component();
		}
	}
}

void GameObject::Update()
{
	for (Component* component : components)
	{
		if (component != nullptr)
		{
			component->Update();
		}
	}
}

void GameObject::AddComponent(Component* component)
{
	if (component != nullptr)
	{
		component->Init(this);
		components.push_back(component);
	}
}

const Transform& GameObject::GetTransform() const
{
	return transform;
}

void GameObject::SetPosition(glm::vec3 position)
{
	transform.position = position;
}

void GameObject::SetRotation(glm::vec3 eulerAngles)
{
	transform.rotation = eulerAngles;
}

void GameObject::SetScale(glm::vec3 scale)
{
	transform.scale = scale;
}