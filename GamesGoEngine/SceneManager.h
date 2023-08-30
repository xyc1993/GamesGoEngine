#pragma once
#include "Scene.h"

namespace GamesGoEngine
{
	class SceneManager
	{
	public:
		static void SetActiveScene(Scene* scene);
		static Scene* GetActiveScene();

	private:
		static Scene* activeScene;
	};
}