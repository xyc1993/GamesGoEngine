#include "GraphicsSettingsPanel.h"

#include <imgui.h>

#include "RenderingManager.h"
#include "WindowManager.h"

namespace GamesGoEngine
{
	GraphicsSettingsPanel::GraphicsSettingsPanel()
	{

	}

	void GraphicsSettingsPanel::Draw()
	{
		ImGui::Begin("Graphics Settings");

		bool vsyncEnabled = WindowManager::IsVSyncEnabled();
		if (ImGui::Checkbox("V-Sync", &vsyncEnabled))
		{
			WindowManager::EnableVSync(vsyncEnabled);
		}

		bool postProcessingEnabled = RenderingManager::IsPostProcessingEnabled();
		if (ImGui::Checkbox("Post Processing", &postProcessingEnabled))
		{
			RenderingManager::EnablePostProcessing(postProcessingEnabled);
		}

		bool hdrToneMappingAndGammaEnabled = RenderingManager::IsHDRToneMappingAndGammaEnabled();
		if (ImGui::Checkbox("HDR tone mapping and gamma correction", &hdrToneMappingAndGammaEnabled))
		{
			RenderingManager::EnableHDRToneMappingAndGamma(hdrToneMappingAndGammaEnabled);
		}
		if (hdrToneMappingAndGammaEnabled)
		{
			float gamma = RenderingManager::GetGamma();
			if (ImGui::SliderFloat("Gamma", &gamma, 0.0f, 5.0f))
			{
				RenderingManager::SetGamma(gamma);
			}

			float exposure = RenderingManager::GetExposure();
			if (ImGui::SliderFloat("Exposure", &exposure, 0.0f, 5.0f))
			{
				RenderingManager::SetExposure(exposure);
			}
		}

		bool bloomEnabled = RenderingManager::IsBloomEnabled();
		if (ImGui::Checkbox("Bloom", &bloomEnabled))
		{
			RenderingManager::EnableBloom(bloomEnabled);
		}
		if (bloomEnabled)
		{
			float bloomStrength = RenderingManager::GetBloomStrength();
			if (ImGui::SliderFloat("Bloom strength", &bloomStrength, 0.0f, 1.0f))
			{
				RenderingManager::SetBloomStrength(bloomStrength);
			}

			int bloomMipChainLength = RenderingManager::GetBloomMipChainLength();
			if (ImGui::SliderInt("Bloom mip chain length", &bloomMipChainLength, 1, 10))
			{
				RenderingManager::SetBloomMipChainLength(bloomMipChainLength);
			}
		}

		bool ssaoEnabled = RenderingManager::IsSSAOEnabled();
		if (ImGui::Checkbox("SSAO", &ssaoEnabled))
		{
			RenderingManager::EnableSSAO(ssaoEnabled);
		}

		constexpr size_t shadowMapResolutionsNumber = 5;
		const char* shadowMaps[shadowMapResolutionsNumber] = { "256", "512", "1024", "2048", "4096" };
		static const char* currentShadowMap = shadowMaps[2];
		if (ImGui::BeginCombo("Shadow map res", currentShadowMap))
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

		bool aaEnabled = RenderingManager::IsAntiAliasingEnabled();
		if (ImGui::Checkbox("Anti-Aliasing", &aaEnabled))
		{
			RenderingManager::EnableAntiAliasing(aaEnabled);
		}
		if (aaEnabled)
		{
			// AA algorithm combo
			constexpr size_t aaAlgorithmsNumber = 1;
			const char* aaAlgorithms[aaAlgorithmsNumber] = { "FXAA" };
			static const char* currentAntiAliasingAlgorithm = aaAlgorithms[0];
			if (ImGui::BeginCombo("AA algorithm", currentAntiAliasingAlgorithm))
			{
				for (int i = 0; i < aaAlgorithmsNumber; i++)
				{
					const bool isSelected = (currentAntiAliasingAlgorithm == aaAlgorithms[i]);
					if (ImGui::Selectable(aaAlgorithms[i], isSelected))
					{
						currentAntiAliasingAlgorithm = aaAlgorithms[i];
						if (i == 0)
						{
							RenderingManager::SetAntiAliasingAlgorithm(AntiAliasingAlgorithm::FXAA);
						}
					}

				}
				ImGui::EndCombo();
			}

			// Settings for specific algorithms
			if (RenderingManager::GetAntiAliasingAlgorithm() == AntiAliasingAlgorithm::FXAA)
			{
				float contrastThreshold = RenderingManager::GetFXAAContrastThreshold();
				if (ImGui::SliderFloat("FXAA contrast threshold", &contrastThreshold, 0.0312f, 0.0833f))
				{
					RenderingManager::SetFXAAContrastThreshold(contrastThreshold);
				}

				float relativeThreshold = RenderingManager::GetFXAARelativeThreshold();
				if (ImGui::SliderFloat("FXAA relative threshold", &relativeThreshold, 0.063f, 0.333f))
				{
					RenderingManager::SetFXAARelativeThreshold(relativeThreshold);
				}
			}
		}

		ImGui::End();
	}
}