#include "EditorPanel.h"

#include <imgui.h>

namespace GamesGoEngine
{
	EditorPanel::EditorPanel()
	{
		posWidthMultiplier = 0.0f;
		posHeightMultiplier = 0.0f;
		sizeWidthMultiplier = 0.5f;
		sizeHeightMultiplier = 0.5f;
	}

	void EditorPanel::DrawToWindowDimensions(float windowWidth, float windowHeight)
	{
		ImGui::SetNextWindowPos(ImVec2(posWidthMultiplier * windowWidth, posHeightMultiplier * windowHeight));
		ImGui::SetNextWindowSize(ImVec2(sizeWidthMultiplier * windowWidth, sizeHeightMultiplier * windowHeight));
		Draw();
	}

	void EditorPanel::SetDrawingMultipliers(float posWidthMul, float posHeightMul, float sizeWidthMul, float sizeHeightMul)
	{
		posWidthMultiplier = posWidthMul;
		posHeightMultiplier = posHeightMul;
		sizeWidthMultiplier = sizeWidthMul;
		sizeHeightMultiplier = sizeHeightMul;
	}
}