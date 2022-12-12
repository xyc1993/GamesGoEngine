#include "GraphicsSettingsUI.h"

#include "imgui.h"
#include "RenderingManager.h"
#include "WindowManager.h"

void GraphicsSettingsUI::Draw()
{
	ImGui::Begin("Graphics Settings");

	bool vsyncEnabled = WindowManager::IsVSyncEnabled();
	if (ImGui::Checkbox("V-Sync Enabled", &vsyncEnabled))
	{
		WindowManager::EnableVSync(vsyncEnabled);
	}

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

	float gamma = RenderingManager::GetGamma();
	if (ImGui::SliderFloat("gamma", &gamma, 0.0f, 5.0f))
	{
		RenderingManager::SetGamma(gamma);
	}

	constexpr size_t shadowMapResolutionsNumber = 5;
	const char* shadowMaps[shadowMapResolutionsNumber] = { "256", "512", "1024", "2048", "4096"};
	static const char* currentShadowMap = shadowMaps[2];
	if (ImGui::BeginCombo("shadow map res", currentShadowMap))
	{
		for (int i = 0; i < shadowMapResolutionsNumber; i++)
		{
			const bool isSelected = (currentShadowMap == shadowMaps[i]);
			if (ImGui::Selectable(shadowMaps[i], isSelected))
			{
				currentShadowMap = shadowMaps[i];

				std::stringstream strValue;
				strValue << currentShadowMap;

				unsigned int shadowMapResInt;
				strValue >> shadowMapResInt;
				
				RenderingManager::SetShadowMapResolution(shadowMapResInt);
			}
				
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}
