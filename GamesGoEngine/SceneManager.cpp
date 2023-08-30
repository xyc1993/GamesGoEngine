#include "SceneManager.h"

namespace GamesGoEngine
{
	Scene* SceneManager::activeScene = nullptr;

	void SceneManager::SetActiveScene(Scene* scene)
	{
		activeScene = scene;
	}

	Scene* SceneManager::GetActiveScene()
	{
		return activeScene;
	}
}