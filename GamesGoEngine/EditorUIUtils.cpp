#include "EditorUIUtils.h"

#include <imgui.h>

#include "AssetsManager.h"

namespace GamesGoEngine
{
	std::string EditorUIUtils::TryGetDropTargetAssetPath(AssetType requestedAssetType)
	{
		std::string dropAssetPath = "";
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserAsset"))
			{
				int* assetId = static_cast<int*>(payload->Data);
				Asset* asset = AssetsManager::GetAsset(*assetId);
				if (asset != nullptr && asset->GetType() == requestedAssetType)
				{
					dropAssetPath = asset->GetPath();
				}
			}
			ImGui::EndDragDropTarget();
		}
		return dropAssetPath;
	}
}