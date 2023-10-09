#include "Asset.h"

namespace GamesGoEngine
{
	Asset::Asset()
	{
		type = AssetType::Unsupported;
		name = "UNKNOWN NAME";
	}

	Asset::Asset(AssetType type, std::string name)
	{
		this->type = type;
		this->name = name;
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