#include "GameObject.h"

#include "Scene.h"

GameObject::GameObject()
{
	components.clear();
	transform = new Transform();
	AddComponent(transform);
}

GameObject::~GameObject()
{
	if (parent != nullptr)
	{
		parent->CleanChildren(this, allChildren);
	}
	
	for (auto it = allChildren.begin(); it != allChildren.end(); ++it)
	{
		GameObject* child = *it;
		if (scene != nullptr)
		{
			scene->RemoveGameObject(child);
		}
		else
		{
			delete child;
		}
	}
	allChildren.clear();
	children.clear();

	for (Component* component : components)
	{
		delete component;
	}
	components.clear();
	transform = nullptr;
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

void GameObject::LateUpdate()
{
	for (Component* component : components)
	{
		if (component != nullptr)
		{
			component->LateUpdate();
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
	if (name.size() > NAME_MAX_LENGTH)
	{
		name.resize(NAME_MAX_LENGTH - 1);
	}
	this->name = name;
}

GameObject* GameObject::GetParent() const
{
	return parent;
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

	CalculateParentsNumber(this->parent, parentsNumber);
	for (auto it = allChildren.begin(); it != allChildren.end(); ++it)
	{
		GameObject* child = *it;
		GameObject::CalculateParentsNumber(child->parent, child->parentsNumber);
	}

	transform->UpdateTransformOnParenting();
	UpdateDirectChildrenTransforms();
}

bool GameObject::IsSelected() const
{
	return selected;
}

void GameObject::SetSelected(bool selected)
{
	this->selected = selected;
}

void GameObject::UpdateDirectChildrenTransforms()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i] != nullptr && children[i]->GetTransform() != nullptr)
		{
			children[i]->GetTransform()->UpdateTransformOnParenting();
			children[i]->UpdateDirectChildrenTransforms();
		}
	}
}

int GameObject::GetAllParentsNumber() const
{
	return parentsNumber;
}

void GameObject::SetSceneReferenece(Scene* scene)
{
	this->scene = scene;
}

const std::vector<GameObject*>& GameObject::GetChildren() const
{
	return children;
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

void GameObject::CalculateParentsNumber(GameObject* parent, int& currentParentNumber)
{
	if (parent != nullptr)
	{
		currentParentNumber++;
		CalculateParentsNumber(parent->GetParent(), currentParentNumber);
	}
}

void GameObject::CleanChildren(GameObject* directChild, std::set<GameObject*> childsChildren)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i] == directChild)
		{
			children.erase(children.begin() + i);
			break;
		}
	}

	for (auto it = childsChildren.begin(); it != childsChildren.end(); ++it)
	{
		auto iterator = allChildren.find(*it);
		if (iterator != allChildren.end())
		{
			allChildren.erase(iterator);
		}
	}
}