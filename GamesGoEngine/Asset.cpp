#include "Asset.h"

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
	}

	void Asset::Unload()
	{

	}

	AssetType Asset::GetType() const
	{
		return type;
	}

	std::string Asset::GetName() const
	{
		return name;
	}
}