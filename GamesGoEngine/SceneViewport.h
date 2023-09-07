#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

namespace GamesGoEngine
{
	class GameObject;

	class SceneViewport
	{
	public:
		void static Draw(GameObject* selectedSceneObject, ImGuizmo::OPERATION transformOperation, float& viewportPosX, float& viewportPosY);
	};
}