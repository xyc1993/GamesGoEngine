#include "GraphicsSettingsUI.h"

#include "imgui.h"
#include "RenderingManager.h"

void GraphicsSettingsUI::Draw()
{
	ImGui::Begin("Graphics Settings");

	bool msaaEnabled = RenderingManager::IsMSAAEnabled();
	if (ImGui::Checkbox("MSAA Enabled", &msaaEnabled))
	{
		RenderingManager::EnableMSAA(msaaEnabled);
	}

	if (msaaEnabled)
	{
		int msaaSamples = RenderingManager::GetMSAASamplesNumber();
		if (ImGui::SliderInt("MSAA samples number", &msaaSamples, 1, RenderingManager::GetMaxMSAASamplesNumber()))
		{
			RenderingManager::SetMSAASamplesNumber(msaaSamples);
		}
	}

	ImGui::End();
}