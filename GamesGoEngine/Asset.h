#pragma once

#include <string>

namespace GamesGoEngine
{
	enum class AssetType
	{
		Texture,
		Unsupported
	};

	class Asset
	{
	public:
		Asset();
		Asset(AssetType type, std::string name);

		AssetType GetType() const;
		std::string GetName() const;

	private:
		AssetType type;
		std::string name;
	};
}