#include "Scene.h"

Scene::Scene()
{
	sceneObjects.clear();
}

Scene::~Scene()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
	}
	sceneObjects.clear();
}

void Scene::AddGameObject(GameObject* gameObject)
{
	sceneObjects.push_back(gameObject);
}

void Scene::Update()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (sceneObjects[i] != nullptr)
		{
			sceneObjects[i]->Update();
		}
	}
}