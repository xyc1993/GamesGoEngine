#include "Scene.h"

#include "CamerasManager.h"
#include "PostProcessRenderer.h"
#include "RenderingManager.h"

namespace GamesGoEngine
{
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
			GameObject* go = it->second;
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
		CamerasManager::RemoveSceneCameras();
	}

	void Scene::AddGameObject(GameObject* gameObject)
	{
		gameObject->SetSceneReferenece(this);
		sceneObjects[gameObject->GetObjectId()] = gameObject;
		if (gameObject->GetName().length() == 0)
		{
			std::string name = "GameObject_";
			name.append(std::to_string(sceneObjects.size() - 1));
			gameObject->SetName(name);
		}
	}

	void Scene::RemoveGameObject(GameObject* gameObject)
	{
		sceneObjects.erase(gameObject->GetObjectId());
		delete gameObject;
	}

	GameObject* Scene::GetGameObjectWithId(int id)
	{
		if (sceneObjects.find(id) == sceneObjects.end())
		{
			return nullptr;
		}
		return sceneObjects[id];
	}

	// TODO: Cache this!
	GameObject* Scene::GetSelectedGameObject()
	{
		for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			GameObject* gameObject = it->second;
			if (gameObject->IsSelected())
			{
				return gameObject;
			}
		}
		return nullptr;
	}

	void Scene::Update()
	{
		for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			GameObject* gameObject = it->second;
			if (gameObject != nullptr)
			{
				gameObject->Update();
			}
		}

		for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			GameObject* gameObject = it->second;
			if (gameObject != nullptr)
			{
				gameObject->LateUpdate();
			}
		}
	}

	const std::map<int, GameObject*>& Scene::GetSceneObjects() const
	{
		return sceneObjects;
	}
}