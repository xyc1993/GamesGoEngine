#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

namespace GamesGoEngine
{
	class GameObject;

	class SceneViewport
	{
	public:
		SceneViewport();

		void Draw(GameObject* selectedSceneObject);
		void SetTransformOperation(ImGuizmo::OPERATION transformOperation);
		void SelectGameObjectAt(int x, int y);
		
	private:
		ImGuizmo::OPERATION currentTransformOperation;
		int viewportPanelPosX;
		int viewportPanelPosY;
		int viewportTextureWidth;
		int viewportTextureHeight;
	};
}