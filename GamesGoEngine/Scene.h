#pragma once

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
		GameObject* GetSelectedGameObject();
		void Update();
		const std::set<GameObject*>& GetSceneObjects() const;

	private:
		std::set<GameObject*> sceneObjects;
	};
}