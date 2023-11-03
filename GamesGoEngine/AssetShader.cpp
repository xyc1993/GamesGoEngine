#include "AssetShader.h"

namespace GamesGoEngine
{
	void AssetShader::Load(AssetType type, std::string name, std::string path)
	{
		Asset::Load(type, name, path);

		// TODO: get meta data regarding uniforms (needed so that material properties panel knows what can be edited)
	}
}