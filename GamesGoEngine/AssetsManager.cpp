#include "AssetsManager.h"

#include <filesystem>

#include "AssetTexture.h"

namespace GamesGoEngine
{
	AssetsManager* AssetsManager::instance = nullptr;

	AssetsManager::AssetsManager()
	{
		loadedAssets.clear();
		selectedAsset = nullptr;
	}

	AssetsManager::~AssetsManager()
	{
		for (auto it = loadedAssets.begin(); it != loadedAssets.end(); ++it)
		{
			it->second->Unload();
		}
	}

	AssetsManager* AssetsManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new AssetsManager();
		}
		return instance;
	}

	void AssetsManager::SelectAsset(std::string path)
	{
		GetInstance()->SelectAssetInternal(path);
	}

	Asset* AssetsManager::GetSelectedAsset()
	{
		return GetInstance()->selectedAsset;
	}

	AssetType AssetsManager::GetType(std::string path)
	{
		std::filesystem::path filePath = path;
		const std::string extension = filePath.extension().string();

		if (extension == ".png" ||
			extension == ".jpg" ||
			extension == ".tga")
		{
			return AssetType::Texture;
		}

		return AssetType::Unsupported;
	}

	void AssetsManager::SelectAssetInternal(std::string path)
	{
		if (loadedAssets.find(path) != loadedAssets.end())
		{
			selectedAsset = loadedAssets[path];
		}
		else
		{
			LoadAsset(path);
			selectedAsset = loadedAssets[path];
		}
	}

	void AssetsManager::LoadAsset(std::string path)
	{
		if (loadedAssets.find(path) == loadedAssets.end())
		{
			std::filesystem::path filePath = path;
			const std::string filename = filePath.filename().string();

			const AssetType assetType = GetType(path);
			Asset* newAsset = nullptr;

			switch (assetType)
			{
			case AssetType::Texture:
				{
				newAsset = new AssetTexture();
				break;
				}
			case AssetType::Unsupported:
				{
				newAsset = new Asset();
				break;
				}
			}

			newAsset->Load(assetType, filename, filePath.string());			
			loadedAssets[path] = newAsset;
		}
	}
}