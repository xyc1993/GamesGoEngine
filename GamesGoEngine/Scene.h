#pragma once

#include <map>

#include "GameObject.h"

namespace GamesGoEngine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void AddGameObject(GameObject* gameObject);
		void RemoveGameObject(GameObject* gameObject);
		GameObject* GetGameObjectWithId(int id);
		void SelectGameObject(GameObject* gameObject);
		GameObject* GetSelectedGameObject();
		void Update();
		const std::map<int, GameObject*>& GetSceneObjects() const;

	private:
		std::map<int, GameObject*> sceneObjects;
	};
}