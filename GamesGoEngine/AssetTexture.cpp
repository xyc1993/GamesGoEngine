#include "AssetTexture.h"

#include <fstream>

#include "TextureLoader.h"

namespace GamesGoEngine
{
	void AssetTexture::Load(AssetType type, std::string name, std::string path)
	{
		Asset::Load(type, name, path);

		std::ifstream propertiesFile(propertiesPath);
		if (propertiesFile.good())
		{
			// TODO: write more elegant file read
			for (int i = 0; i <= 5; i++)
			{
				std::string s;
				std::getline(propertiesFile, s);
				if (i == 3)
				{
					int value = std::stoi(s);
					transparencyEnabled = value;
				}
				if (i == 5)
				{
					int value = std::stoi(s);
					sRGB = value;
				}
			}
		}
		else
		{
			// default texture settings on load
			transparencyEnabled = false;
			sRGB = true;
		}
		propertiesFile.close();

		loadedTexture = TextureLoader::LoadTexture(width, height, path.c_str(), transparencyEnabled, sRGB);
	}

	void AssetTexture::Unload()
	{
		Asset::Unload();

		TextureLoader::UnloadTexture(loadedTexture);
	}

	void AssetTexture::Save()
	{
		std::ofstream outfile(propertiesPath);
		outfile << "Name" << "\n";
		outfile << GetName() << "\n";
		outfile << "transparencyEnabled" << "\n";
		outfile << transparencyEnabled << "\n";
		outfile << "sRGB" << "\n";
		outfile << sRGB << "\n";
		outfile.close();
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
}