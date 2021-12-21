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
		delete component;
	}
	components.clear();

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

std::string GameObject::GetName() const
{
	return name;
}

void GameObject::SetName(std::string name)
{
	this->name = name;
}