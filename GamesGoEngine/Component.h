#pragma once

class GameObject;

class Component
{
public:
	virtual ~Component() = default;
	virtual void Init(GameObject* owner);
	virtual void Update() = 0;
	virtual void LateUpdate() {};
	virtual void OnSelected() {};
	virtual void OnDeselected() {};
	GameObject* GetOwner() const;
	static bool IsValid(const Component* component);

protected:
	GameObject* owner;
};