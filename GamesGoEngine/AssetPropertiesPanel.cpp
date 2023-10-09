#include "AssetPropertiesPanel.h"

#include <imgui.h>

#include "AssetsManager.h"

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
				break;
			case AssetType::Unsupported: 
				ImGui::Text("Unsupported");
				break;
			}
		}

		ImGui::End();
	}
}