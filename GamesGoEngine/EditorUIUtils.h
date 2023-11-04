#pragma once

#include <string>

#include "Asset.h"

namespace GamesGoEngine
{
	class EditorUIUtils
	{
	public:
		static std::string TryGetDropTargetAssetPath(AssetType requestedAssetType);
	};
}