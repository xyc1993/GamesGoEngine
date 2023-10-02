#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <filesystem>

namespace GamesGoEngine
{
	ContentBrowserPanel::ContentBrowserPanel()
	{
		// TODO: in the future directory iterator should take project path from project file and iterate files in project directory instead of "res"
		currentDirectory = resDirectory;
	}

	void ContentBrowserPanel::Draw()
	{
		ImGui::Begin("Content Browser");

		ImGui::Text("%s", currentDirectory.string().c_str());

		if (currentDirectory != std::filesystem::path(resDirectory))
		{
			if (ImGui::Button("<-"))
			{
				currentDirectory = currentDirectory.parent_path();
			}
		}

		for (auto& p : std::filesystem::directory_iterator(currentDirectory))
		{
			const std::string path = p.path().filename().string();
			if (p.is_directory())
			{
				if (ImGui::Button(path.c_str()))
				{
					currentDirectory /= p.path().filename();
				}
			}
			else // file
			{
				if (ImGui::Button(path.c_str()))
				{
					
				}
			}
		}

		ImGui::End();
	}
}