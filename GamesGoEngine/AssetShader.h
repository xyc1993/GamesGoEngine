#pragma once

#include "Asset.h"

namespace GamesGoEngine
{
	class AssetShader : public Asset
	{
	public:
		virtual void Load(AssetType type, std::string name, std::string path) override;
	};
}