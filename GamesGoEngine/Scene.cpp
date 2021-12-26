#include "Scene.h"

Scene::Scene()
{
	sceneObjects.clear();
}

Scene::~Scene()
{
	for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		if (*it != nullptr)
		{
			delete *it;
		}
	}
	sceneObjects.clear();
}

void Scene::AddGameObject(GameObject* gameObject)
{
	gameObject->SetSceneReferenece(this);
	sceneObjects.insert(gameObject);
	if (gameObject->GetName().length() == 0)
	{
		std::string name = "GameObject_";
		name.append(std::to_string(sceneObjects.size() - 1));
		gameObject->SetName(name);
	}
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
	auto iterator = sceneObjects.find(gameObject);
	if (iterator != sceneObjects.end())
	{
		delete *iterator;
		sceneObjects.erase(iterator);
	}
}

void Scene::Update()
{
	for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		if (*it != nullptr)
		{
			(*it)->Update();
		}
	}
}

const std::set<GameObject*>& Scene::GetSceneObjects() const
{
	return sceneObjects;
}