#include "Asset.h"

#include <fstream>

#include "AssetsManager.h"

namespace GamesGoEngine
{
	Asset::Asset()
	{
		type = AssetType::Unsupported;
		name = "UNKNOWN NAME";
	}

	void Asset::Load(AssetType type, std::string name, std::string path)
	{
		this->type = type;
		this->name = name;
		this->path = path;

		this->propertiesPath = path;
		this->propertiesPath.append(AssetsManager::GetAssetPropertiesFileExtension());
	}

	void Asset::Unload()
	{

	}

	void Asset::Save()
	{
		std::ofstream outfile(propertiesPath);
		outfile << "Name" << "\n";
		outfile << GetName() << "\n";
		outfile.close();
	}

	AssetType Asset::GetType() const
	{
		return type;
	}

	std::string Asset::GetName() const
	{
		return name;
	}

	std::string Asset::GetPath() const
	{
		return path;
	}

	std::string Asset::GetPropertiesFilePath() const
	{
		return propertiesPath;
	}
}