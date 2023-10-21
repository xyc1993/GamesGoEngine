#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <filesystem>

#include "AssetsManager.h"
#include "TextureLoader.h"

namespace GamesGoEngine
{
	ContentBrowserPanel::ContentBrowserPanel()
	{
		// used later for cleanup
		backButtonTexture = TextureLoader::LoadTexture("res/icons/back.png", true, true);
		fileButtonTexture = TextureLoader::LoadTexture("res/icons/file.png", true, true);
		folderButtonTexture = TextureLoader::LoadTexture("res/icons/folder.png", true, true);

		// used to avoid casting on drawing
		backButtonTextureID = (ImTextureID)backButtonTexture;
		fileButtonTextureID = (ImTextureID)fileButtonTexture;
		folderButtonTextureID = (ImTextureID)folderButtonTexture;

		padding = 16.0f;
		thumbnailSize = 64.0f;
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
		TextureLoader::UnloadTexture(backButtonTexture);
		TextureLoader::UnloadTexture(fileButtonTexture);
		TextureLoader::UnloadTexture(folderButtonTexture);
	}

	void ContentBrowserPanel::Draw()
	{
		ImGui::Begin("Content Browser");

		const std::string projectPath = AssetsManager::GetProjectPath();
		if (projectPath.empty())
		{
			ImGui::Text("Please open project file");
			ImGui::End();
			return;
		}

		// Reset current directory if project file path has changed
		const std::string projectFilePath = AssetsManager::GetProjectPath();
		if (currentProjectFilePath != projectFilePath)
		{
			currentProjectFilePath = projectFilePath;
			currentDirectory = projectPath;
		}

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

		if (currentDirectory != std::filesystem::path(projectPath))
		{
			if (ImGui::ImageButton(backButtonTextureID, buttonsThumbnailSize))
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
				if (ImGui::ImageButton(filename.c_str(), folderButtonTextureID, buttonsThumbnailSize))
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
			const std::string fileExtension = p.path().filename().extension().string();

			if (!p.is_directory() && (fileExtension != AssetsManager::GetAssetPropertiesFileExtension()))
			{
				if (ImGui::ImageButton(filename.c_str(), fileButtonTextureID, buttonsThumbnailSize))
				{
					AssetsManager::SelectAsset(p.path().string());
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