#include "GameObject.h"

GameObject::GameObject()
{
	transform.position = glm::vec3(0.0f);
	transform.scale = glm::vec3(1.0f);

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

glm::vec3 GameObject::GetPosition() const
{
	return transform.position;
}

void GameObject::SetPosition(glm::vec3 position)
{
	transform.position = position;
}