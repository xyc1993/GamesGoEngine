#pragma once

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class GameObject;

	class PropertiesUI : EditorPanel
	{
	public:
		PropertiesUI();

		virtual void Draw() override;

	private:
		static void DrawNameInputField(GameObject* selectedGameObject);
		static void DrawTransformField(GameObject* selectedGameObject);
	};
}