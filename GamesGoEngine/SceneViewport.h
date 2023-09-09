#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class GameObject;

	class SceneViewport : public EditorPanel
	{
	public:
		SceneViewport();

		virtual void Draw() override;
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