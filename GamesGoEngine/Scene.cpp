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
	gameObject->SetSceneReferenece(this);
	sceneObjects.push_back(gameObject);
	if (gameObject->GetName().length() == 0)
	{
		std::string name = "GameObject_";
		name.append(std::to_string(sceneObjects.size() - 1));
		gameObject->SetName(name);
	}
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return;
	}

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (sceneObjects[i] == gameObject)
		{
			delete sceneObjects[i];
			// just in case set the nullptr to the place of the deleted game object
			sceneObjects[i] = nullptr;
			// finally, after the proper cleanup, erase the element from the vector
			sceneObjects.erase(sceneObjects.begin() + i);
		}
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