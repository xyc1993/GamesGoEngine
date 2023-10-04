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
		std::filesystem::path currentDirectory;
		const std::filesystem::path resDirectory = "res";

		ImTextureID backButtonTexture;
		ImTextureID fileButtonTexture;
		ImTextureID folderButtonTexture;

		float padding;
		float thumbnailSize;
	};
}