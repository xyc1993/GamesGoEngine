#pragma once

#include <imgui.h>
#include <filesystem>

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class ContentBrowserPanel : public EditorPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		virtual void Draw() override;

	private:
		std::filesystem::path currentProjectFilePath;
		std::filesystem::path currentDirectory;

		unsigned int backButtonTexture;
		unsigned int fileButtonTexture;
		unsigned int folderButtonTexture;

		ImTextureID backButtonTextureID;
		ImTextureID fileButtonTextureID;
		ImTextureID folderButtonTextureID;

		float padding;
		float thumbnailSize;
	};
}