#include "GameObject.h"

GameObject::GameObject()
{
	transform = new Transform();
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
	delete transform;
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

Transform* GameObject::GetTransform() const
{
	return transform;
}