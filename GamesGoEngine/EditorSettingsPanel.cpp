#include "EditorSettingsPanel.h"

#include <imgui.h>

namespace GamesGoEngine
{
	void EditorSettingsPanel::Draw()
	{
		ImGui::Begin("Editor Settings");
		
		if (ImGui::SliderFloat("Font scale", &fontGlobalScale, 0.5f, 3.0f))
		{
			ImGui::GetIO().FontGlobalScale = fontGlobalScale;
		}

		ImGui::End();
	}
}