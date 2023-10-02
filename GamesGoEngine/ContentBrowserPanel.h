#pragma once

#include <filesystem>

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class ContentBrowserPanel : public EditorPanel
	{
	public:
		ContentBrowserPanel();

		virtual void Draw() override;

	private:
		std::filesystem::path currentDirectory;
		const std::filesystem::path resDirectory = "res";
	};
}