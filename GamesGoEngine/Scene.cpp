#include "Scene.h"

Scene::Scene()
{
	sceneObjects.clear();
}

Scene::~Scene()
{
	// delete only the top most game objects in the scene since their destructor will delete their children as well
	std::vector<GameObject*> markedForDeletion;
	for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		GameObject* go = *it;
		if (go != nullptr && go->GetAllParentsNumber() == 0)
		{
			markedForDeletion.push_back(go);
		}
	}

	for (int i = 0; i < markedForDeletion.size(); i++)
	{
		RemoveGameObject(markedForDeletion[i]);
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