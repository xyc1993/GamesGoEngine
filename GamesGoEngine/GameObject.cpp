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

void GameObject::SetParent(GameObject* parent)
{
	// if 'parent' is one of GameObject's children, don't set it as this->parent
	if (allChildren.find(parent) != allChildren.end())
	{
		return;
	}

	if (parent == nullptr)
	{
		RemoveParent();
	}
	else
	{
		if (this->parent != nullptr)
		{
			RemoveParent();
		}
		SetNewParent(parent);
	}
}

void GameObject::RemoveParent()
{
	// remove child's children & child from parent's allChildren
	for (auto it = allChildren.begin(); it != allChildren.end(); ++it)
	{
		GameObject* child = *it;
		this->parent->allChildren.erase(child);
	}
	this->parent->allChildren.erase(this);

	// remove child from parent's direct children container
	for (int i = 0; i < this->parent->children.size(); i++)
	{
		if (this->parent->children[i] == this)
		{
			this->parent->children.erase(this->parent->children.begin() + i);
			break;
		}
	}

	// finally remove the parent reference
	this->parent = nullptr;
}

void GameObject::SetNewParent(GameObject* parent)
{
	// set the parent & add child's children to parent's children
	this->parent = parent;

	this->parent->children.push_back(this);
	this->parent->allChildren.insert(this);

	for (auto it = allChildren.begin(); it != allChildren.end(); ++it)
	{
		GameObject* child = *it;
		this->parent->allChildren.insert(child);
	}
}