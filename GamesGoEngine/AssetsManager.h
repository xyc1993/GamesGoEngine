#pragma once

#include <map>
#include <string>

#include "Asset.h"

namespace GamesGoEngine
{
	class AssetsManager
	{
	private:
		AssetsManager();

		static AssetsManager* instance;
		static AssetsManager* GetInstance();

	public:
		static void SelectAsset(std::string path);
		static Asset* GetSelectedAsset();
		// Determines asset type based on file's path
		static AssetType GetType(std::string path);
		
	private:
		void SelectAssetInternal(std::string path);
		void LoadAsset(std::string path);

		// key is asset path
		std::map<std::string, Asset*> loadedAssets;
		Asset* selectedAsset;
	};
}