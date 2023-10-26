#include "AssetTexture.h"

#include <fstream>

#include "TextureLoader.h"

namespace GamesGoEngine
{
	void AssetTexture::Load(AssetType type, std::string name, std::string path)
	{
		Asset::Load(type, name, path);

		loadedTexture = TextureLoader::LoadTexture(width, height, path.c_str(), transparencyEnabled, sRGB);
	}

	void AssetTexture::Unload()
	{
		Asset::Unload();

		TextureLoader::UnloadTexture(loadedTexture);
	}

	unsigned AssetTexture::GetTexture() const
	{
		return loadedTexture;
	}

	bool AssetTexture::IsTransparencyEnabled() const
	{
		return transparencyEnabled;
	}

	bool AssetTexture::IsSRGB() const
	{
		return sRGB;
	}

	int AssetTexture::GetWidth() const
	{
		return width;
	}

	int AssetTexture::GetHeight() const
	{
		return height;
	}

	void AssetTexture::EnableTransparency(bool enable)
	{
		if (transparencyEnabled != enable)
		{
			transparencyEnabled = enable;
			// unload old texture
			TextureLoader::UnloadTexture(loadedTexture);
			// load texture with new settings
			loadedTexture = TextureLoader::LoadTexture(width, height, path.c_str(), transparencyEnabled, sRGB);
		}
	}

	void AssetTexture::EnableSRGB(bool enable)
	{
		if (sRGB != enable)
		{
			sRGB = enable;
			// unload old texture
			TextureLoader::UnloadTexture(loadedTexture);
			// load texture with new settings
			loadedTexture = TextureLoader::LoadTexture(width, height, path.c_str(), transparencyEnabled, sRGB);
		}
	}

	void AssetTexture::InitMetaData()
	{
		metaData.className = CLASS_NAME(AssetTexture);

		ADD_FIELD_META_DATA(metaData, bool, transparencyEnabled, this->transparencyEnabled);
		ADD_FIELD_META_DATA(metaData, bool, sRGB, this->sRGB);
	}
}