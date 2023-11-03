#include "AssetsManager.h"

#include <filesystem>

#include "AssetMaterial.h"
#include "AssetProject.h"
#include "AssetShader.h"
#include "AssetTexture.h"

namespace GamesGoEngine
{
	AssetsManager* AssetsManager::instance = nullptr;

	AssetsManager::AssetsManager()
	{
		loadedAssets.clear();
		selectedAsset = nullptr;
		currentProject = nullptr;
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

	Asset* AssetsManager::GetAsset(std::string path)
	{
		return GetInstance()->GetAssetInternal(path);
	}

	Asset* AssetsManager::GetAsset(int id)
	{
		return GetInstance()->GetAssetInternal(id);
	}

	AssetType AssetsManager::GetType(std::string path)
	{
		std::filesystem::path filePath = path;
		const std::string extension = filePath.extension().string();

		if (extension == ".mat")
		{
			return AssetType::Material;
		}
		if (extension == ".ggproject")
		{
			return AssetType::Project;
		}
		if (extension == ".vert" ||
			extension == ".frag" ||
			extension == ".hlsl" ||
			extension == ".glsl")
		{
			return AssetType::Shader;
		}
		if (extension == ".png" ||
			extension == ".jpg" ||
			extension == ".tga")
		{
			return AssetType::Texture;
		}

		return AssetType::Unsupported;
	}

	void AssetsManager::LoadAsset(std::string path)
	{
		GetInstance()->LoadAssetInternal(path);
	}

	std::string AssetsManager::GetProjectPath()
	{
		return GetInstance()->GetProjectPathInternal();
	}

	std::string AssetsManager::GetProjectFilePath()
	{
		return GetInstance()->GetProjectFilePathInternal();
	}

	void AssetsManager::SaveAllAssets()
	{
		GetInstance()->SaveAllAssetsInternal();
	}

	std::string AssetsManager::GetAssetPropertiesFileExtension()
	{
		return ".ggproperties";
	}

	void AssetsManager::SelectAssetInternal(std::string path)
	{
		const int id = GetIdFromPath(path);
		if (loadedAssets.find(id) != loadedAssets.end())
		{
			selectedAsset = loadedAssets[id];
		}
		else
		{
			LoadAssetInternal(path);
			selectedAsset = loadedAssets[id];
		}
	}

	Asset* AssetsManager::GetAssetInternal(std::string path)
	{
		const int id = GetIdFromPath(path);
		if (loadedAssets.find(id) != loadedAssets.end())
		{
			return loadedAssets[id];
		}
		else
		{
			LoadAssetInternal(path);
			return loadedAssets[id];
		}
	}

	Asset* AssetsManager::GetAssetInternal(int id)
	{
		if (loadedAssets.find(id) != loadedAssets.end())
		{
			return loadedAssets[id];
		}
		return nullptr;
	}

	void AssetsManager::LoadAssetInternal(std::string path)
	{
		const int id = GetIdFromPath(path);
		if (loadedAssets.find(id) == loadedAssets.end())
		{
			std::filesystem::path filePath = path;
			const std::string filename = filePath.filename().string();

			const AssetType assetType = GetType(path);
			Asset* newAsset = nullptr;

			switch (assetType)
			{
			case AssetType::Material:
				{
					newAsset = new AssetMaterial();
					break;
				}
			case AssetType::Project:
				{
					AssetProject* assetProject = new AssetProject();
					if(currentProject != nullptr)
					{
						delete currentProject;
					}
					currentProject = assetProject;
					newAsset = assetProject;
					break;
				}
			case AssetType::Shader:
				{
					newAsset = new AssetShader();
					break;
				}
			case AssetType::Texture:
				{
					newAsset = new AssetTexture();
					break;
				}
			default:
				{
					newAsset = new Asset();
					break;
				}
			}

			newAsset->Load(assetType, filename, filePath.string());
			loadedAssets[id] = newAsset;
		}
	}

	std::string AssetsManager::GetProjectPathInternal() const
	{
		if (currentProject != nullptr)
		{
			return currentProject->GetProjectPath();
		}
		return std::string();
	}

	std::string AssetsManager::GetProjectFilePathInternal() const
	{
		if (currentProject != nullptr)
		{
			return currentProject->GetPath();
		}
		return std::string();
	}

	void AssetsManager::SaveAllAssetsInternal()
	{
		for (auto it = loadedAssets.begin(); it != loadedAssets.end(); ++it)
		{
			it->second->Save();
		}
	}

	int AssetsManager::GetIdFromPath(std::string path)
	{
		int id = 0;
		for (int i = 0; i < path.size(); i++)
		{
			id += static_cast<int>(path[i]);
		}
		return id;
	}
}