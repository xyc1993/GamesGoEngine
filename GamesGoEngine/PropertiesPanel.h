#pragma once

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class GameObject;

	class PropertiesPanel : public EditorPanel
	{
	public:
		PropertiesPanel();

		virtual void Draw() override;

	private:
		static void DrawNameInputField(GameObject* selectedGameObject);
		static void DrawTransformField(GameObject* selectedGameObject);
		static void DrawComponentsFields(GameObject* selectedGameObject);
		static void DrawAddComponentButton(GameObject* selectedGameObject);
	};
}