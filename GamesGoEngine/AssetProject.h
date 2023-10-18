#pragma once

#include "Asset.h"

namespace GamesGoEngine
{
	/*
	 * Asset file containing general project data such as project root directory
	 */
	class AssetProject : public Asset
	{
	public:
		virtual void Load(AssetType type, std::string name, std::string path) override;

		std::string GetProjectPath() const;

	private:
		std::string projectPath;
	};
}
