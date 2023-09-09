#pragma once

namespace GamesGoEngine
{
	class GameObject;

	class PropertiesUI
	{
	public:
		static void Draw();

	private:
		static void DrawNameInputField(GameObject* selectedGameObject);
		static void DrawTransformField(GameObject* selectedGameObject);
	};
}