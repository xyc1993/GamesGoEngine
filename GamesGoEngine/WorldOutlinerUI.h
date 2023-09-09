#pragma once

#include "EditorPanel.h"
#include "Scene.h"

namespace GamesGoEngine
{
	class WorldOutlinerUI : EditorPanel
	{
	public:
		WorldOutlinerUI();

		virtual void Draw() override;

	private:
		static void DrawSceneNodeChildren(Scene* activeScene, GameObject* sceneObject);
		static void HandleDragAndDrop(Scene* activeScene, GameObject* sceneObject);
	};
}