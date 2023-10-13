#pragma once

#include "Asset.h"

namespace GamesGoEngine
{
	class AssetTexture : public Asset
	{
	public:
		virtual void Load(AssetType type, std::string name, std::string path) override;
		virtual void Unload() override;

		unsigned int GetTexture() const;
		bool IsTransparencyEnabled() const;
		bool IsSRGB() const;
		int GetWidth() const;
		int GetHeight() const;

		void EnableTransparency(bool enable);
		void EnableSRGB(bool enable);

	private:
		// handle to the loaded texture
		unsigned int loadedTexture;
		// should load alpha channel
		// TODO: this could be connected with sRGB in the enum (RGB, RGBA, sRGB, sRGBA)
		bool transparencyEnabled;
		// sRGB if true : RGB if false
		// TODO: this should be enum in the future, can't be tackled on renderer refactoring
		bool sRGB;

		int width;
		int height;
	};
}