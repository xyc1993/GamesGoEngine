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

		virtual void Load(AssetType type, std::string name, std::string path);
		virtual void Unload();

		AssetType GetType() const;
		std::string GetName() const;
		
	protected:
		std::string path;

	private:
		AssetType type;
		std::string name;
	};
}