#pragma once

#include <vector>

#include "GameObject.h"

class Scene
{
public:
	Scene();
	~Scene();

	int AddGameObject(GameObject* gameObject);
	void RemoveGameObject(int gameObjectIndex);
	void Update();
	const std::vector<GameObject*>& GetSceneObjects() const;

private:
	std::vector<GameObject*> sceneObjects;
};