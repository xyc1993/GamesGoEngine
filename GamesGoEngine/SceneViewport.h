#pragma once

namespace GamesGoEngine
{
	class GameObject;

	class SceneViewport
	{
	public:
		void static Draw(GameObject* selectedSceneObject, float& viewportPosX, float& viewportPosY);
	};
}