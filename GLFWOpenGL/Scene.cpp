#include "Scene.h"

Scene::Scene()
{
	sceneObjects.clear();
}

Scene::~Scene()
{
	Clear();
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

void Scene::Clear()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
	}
	sceneObjects.clear();
}