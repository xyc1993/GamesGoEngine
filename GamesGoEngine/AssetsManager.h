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
		static Asset* GetAsset(std::string path);
		static Asset* GetAsset(int id);
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
		Asset* GetAssetInternal(std::string path);
		Asset* GetAssetInternal(int id);
		void LoadAssetInternal(std::string path);
		std::string GetProjectPathInternal() const;
		std::string GetProjectFilePathInternal() const;
		void SaveAllAssetsInternal();

	public:
		static int GetIdFromPath(std::string path);

	private:
		// key is unique id made out of path
		std::map<int, Asset*> loadedAssets;
		Asset* selectedAsset;
		AssetProject* currentProject;
	};
}