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
	if (gameObject->GetName().length() == 0)
	{
		std::string name = "GameObject_";
		name.append(std::to_string(sceneObjects.size() - 1));
		gameObject->SetName(name);
	}
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

const std::vector<GameObject*>& Scene::GetSceneObjects() const
{
	return sceneObjects;
}