#pragma once

class GameObject;

class Component
{
public:
	virtual ~Component() = default;
	virtual void Init(GameObject* owner);
	virtual void Update() = 0;
	virtual void LateUpdate() {};

protected:
	GameObject* owner;
};