#include "AssetPropertiesPanel.h"

#include <imgui.h>

#include "AssetsManager.h"
#include "AssetTexture.h"

namespace GamesGoEngine
{
	void AssetPropertiesPanel::Draw()
	{
		ImGui::Begin("Asset Properties");

		Asset* selectedAsset = AssetsManager::GetSelectedAsset();
		if (selectedAsset != nullptr)
		{
			ImGui::Text(selectedAsset->GetName().c_str());
			switch(selectedAsset->GetType())
			{
			case AssetType::Texture:
				ImGui::Text("Texture");
				TryDrawAssetTextureData(selectedAsset);
				break;
			case AssetType::Unsupported: 
				ImGui::Text("Unsupported");
				break;
			}
		}

		ImGui::End();
	}

	void AssetPropertiesPanel::TryDrawAssetTextureData(Asset* asset) const
	{
		if (AssetTexture* textureAsset = dynamic_cast<AssetTexture*>(asset))
		{
			bool transparencyEnabled = textureAsset->IsTransparencyEnabled();
			if (ImGui::Checkbox("Transparency enabled", &transparencyEnabled))
			{
				textureAsset->EnableTransparency(transparencyEnabled);
			}

			bool isSRGB = textureAsset->IsSRGB();
			if (ImGui::Checkbox("sRGB", &isSRGB))
			{
				textureAsset->EnableSRGB(isSRGB);
			}

			ImGui::BeginChild("Texture Image");

			// texture dimensions
			const float width = static_cast<float>(textureAsset->GetWidth());
			const float height = static_cast<float>(textureAsset->GetHeight());

			// image view dimensions, get width based on panel size and scale height based on aspect ratio of the texture
			const float viewWidth = ImGui::GetWindowSize().x;
			const float viewHeight = height / width * viewWidth;

			const ImVec2 imageSize = ImVec2(viewWidth, viewHeight);
			ImGui::Image((void*)textureAsset->GetTexture(), imageSize);

			ImGui::EndChild();

			std::string imageResolutionString;
			imageResolutionString.append(std::to_string(textureAsset->GetWidth()));
			imageResolutionString.append("x");
			imageResolutionString.append(std::to_string(textureAsset->GetHeight()));
			ImGui::Text(imageResolutionString.c_str());
		}
	}
}