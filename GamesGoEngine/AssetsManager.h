#pragma once

#include <map>
#include <string>

#include "Asset.h"

namespace GamesGoEngine
{
	class AssetProject;

	class AssetsManager
	{
	private:
		AssetsManager();
		~AssetsManager();

		static AssetsManager* instance;
		static AssetsManager* GetInstance();

	public:
		static void SelectAsset(std::string path);
		static Asset* GetSelectedAsset();
		// Determines asset type based on file's path
		static AssetType GetType(std::string path);
		static void LoadAsset(std::string path);
		static std::string GetProjectPath();
		static std::string GetProjectFilePath();
		// Saves all assets that were open for editing
		static void SaveAllAssets();
		static std::string GetAssetPropertiesFileExtension();
		
	private:
		void SelectAssetInternal(std::string path);
		void LoadAssetInternal(std::string path);
		std::string GetProjectPathInternal() const;
		std::string GetProjectFilePathInternal() const;
		void SaveAllAssetsInternal();

		// key is asset path
		std::map<std::string, Asset*> loadedAssets;
		Asset* selectedAsset;
		AssetProject* currentProject;
	};
}