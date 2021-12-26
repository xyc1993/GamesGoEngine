#pragma once

#include "GameObject.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AddGameObject(GameObject* gameObject);
	void RemoveGameObject(GameObject* gameObject);
	void Update();
	const std::set<GameObject*>& GetSceneObjects() const;

private:
	std::set<GameObject*> sceneObjects;
};