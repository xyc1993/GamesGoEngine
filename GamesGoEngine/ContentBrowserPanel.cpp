#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <filesystem>

#include "TextureLoader.h"

namespace GamesGoEngine
{
	ContentBrowserPanel::ContentBrowserPanel()
	{
		// TODO: in the future directory iterator should take project path from project file and iterate files in project directory instead of "res"
		currentDirectory = resDirectory;

		backButtonTexture = reinterpret_cast<ImTextureID>(TextureLoader::LoadTexture(const_cast<GLchar*>("res/icons/back.png"), true, true));
		fileButtonTexture = reinterpret_cast<ImTextureID>(TextureLoader::LoadTexture(const_cast<GLchar*>("res/icons/file.png"), true, true));
		folderButtonTexture = reinterpret_cast<ImTextureID>(TextureLoader::LoadTexture(const_cast<GLchar*>("res/icons/folder.png"), true, true));

		padding = 16.0f;
		thumbnailSize = 64.0f;
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
		// TODO: textures should be deleted!
	}

	void ContentBrowserPanel::Draw()
	{
		ImGui::Begin("Content Browser");

		std::string currentDirectoryText = "Current Directory: ";
		currentDirectoryText.append(currentDirectory.string());
		ImGui::Text("%s", currentDirectoryText.c_str());
		
		const float cellSize = padding + thumbnailSize;
		const ImVec2 buttonsThumbnailSize = { thumbnailSize , thumbnailSize };
		const float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = static_cast<int>(panelWidth / cellSize);
		if (columnCount < 1)
		{
			columnCount = 1;
		}

		ImGui::Columns(columnCount, 0, false);

		if (currentDirectory != std::filesystem::path(resDirectory))
		{
			if (ImGui::ImageButton(backButtonTexture, buttonsThumbnailSize))
			{
				currentDirectory = currentDirectory.parent_path();
			}

			ImGui::NextColumn();
		}

		// In order to draw folders first and then files, iterate twice
		// Iterate over folders and draw the image button for folders
		for (auto& p : std::filesystem::directory_iterator(currentDirectory))
		{
			const std::string filename = p.path().filename().string();
			if (p.is_directory())
			{
				if (ImGui::ImageButton(filename.c_str(), folderButtonTexture, buttonsThumbnailSize))
				{
					currentDirectory /= p.path().filename();
				}
				ImGui::TextWrapped(filename.c_str());
				ImGui::NextColumn();
			}			
		}

		// Iterate over files and draw the image button for files
		for (auto& p : std::filesystem::directory_iterator(currentDirectory))
		{
			const std::string filename = p.path().filename().string();
			if (!p.is_directory())
			{
				if (ImGui::ImageButton(filename.c_str(), fileButtonTexture, buttonsThumbnailSize))
				{

				}
				ImGui::TextWrapped(filename.c_str());
				ImGui::NextColumn();
			}
		}

		/*
		ImGui::Columns(1);		
		ImGui::SliderFloat("Padding", &padding, 0.0f, 32.0f);
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.0f, 512.0f);
		*/

		ImGui::End();
	}
}