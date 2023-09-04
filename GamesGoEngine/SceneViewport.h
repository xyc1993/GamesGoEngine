#pragma once

namespace GamesGoEngine
{
	class GameObject;

	class SceneViewport
	{
	public:
		void static Draw(float width, float height, GameObject* selectedSceneObject);
	};
}