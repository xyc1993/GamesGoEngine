#pragma once

#include <vector>

#include "GameObject.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AddGameObject(GameObject* gameObject);
	void Update();
	void Clear();

private:
	std::vector<GameObject*> sceneObjects;
};