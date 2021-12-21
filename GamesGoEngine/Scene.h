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
	const std::vector<GameObject*>& GetSceneObjects() const;

private:
	std::vector<GameObject*> sceneObjects;
};