#include "AssetProject.h"

#include <filesystem>

namespace GamesGoEngine
{
	void AssetProject::Load(AssetType type, std::string name, std::string path)
	{
		Asset::Load(type, name, path);

		projectPath = std::filesystem::path(path).parent_path().string();
	}

	std::string AssetProject::GetProjectPath() const
	{
		return projectPath;
	}
}