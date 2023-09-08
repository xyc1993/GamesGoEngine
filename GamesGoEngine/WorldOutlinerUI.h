#pragma once

#include "Scene.h"

namespace GamesGoEngine
{
	class WorldOutlinerUI
	{
	public:
		static GameObject* Draw();

	private:
		static void DrawSceneNodeChildren(Scene* activeScene, GameObject* sceneObject);
		static void HandleDragAndDrop(Scene* activeScene, GameObject* sceneObject);

		static GameObject* selectedSceneObject;
	};
}