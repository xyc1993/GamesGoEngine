#include "EditorTitleBar.h"

#include <iostream>
#include <fstream>
#include <imgui.h>
#include <GLFW/glfw3.h>

#include "AssetsManager.h"
#include "FileDialogs.h"
#include "TextureLoader.h"
#include "WindowManager.h"

namespace GamesGoEngine
{
	EditorTitleBar::EditorTitleBar()
	{
		isWindowMaximized = false;
		isTitleBarHovered = false;

		// used later for cleanup
		minimizeButtonTexture = TextureLoader::LoadTexture("res/icons/minimize.png", true, true);
		maximizeButtonTexture = TextureLoader::LoadTexture("res/icons/maximize.png", true, true);
		closeButtonTexture = TextureLoader::LoadTexture("res/icons/close.png", true, true);

		// used to avoid casting on drawing
		minimizeButtonTextureID = (ImTextureID)minimizeButtonTexture;
		maximizeButtonTextureID = (ImTextureID)maximizeButtonTexture;
		closeButtonTextureID = (ImTextureID)closeButtonTexture;
	}

	EditorTitleBar::~EditorTitleBar()
	{
		TextureLoader::UnloadTexture(minimizeButtonTexture);
		TextureLoader::UnloadTexture(maximizeButtonTexture);
		TextureLoader::UnloadTexture(closeButtonTexture);
	}

	void EditorTitleBar::Draw()
	{
		if (ImGui::BeginMenuBar())
		{
			// TODO: add logo in top-right corner once it's done
			
			const float titleBarHeight = ImGui::GetFrameHeight() + 1.0f; // +1.0f to avoid button overlap
			const float titleBarWidth = ImGui::GetWindowSize().x;
			float dragZoneWidth = titleBarWidth - 3.0 * titleBarHeight - 10.0f; // -10.0f comes from padding; TODO: do it in more scalable way in case more buttons are added
			
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Project"))
				{
					const std::string projectFilePath = FileDialogs::SaveFile("GamesGo Project (*.ggproject)\0*.ggproject\0");
					if (!projectFilePath.empty())
					{
						// For now, let's just create an empty file
						std::ofstream outfile(projectFilePath);
						outfile.close();
						AssetsManager::LoadAsset(projectFilePath);
					}
				}

				if (ImGui::MenuItem("Open Project"))
				{
					const std::string projectFilePath = FileDialogs::OpenFile("GamesGo Project (*.ggproject)\0*.ggproject\0");
					if (!projectFilePath.empty())
					{
						AssetsManager::LoadAsset(projectFilePath);
					}
				}

				if (ImGui::MenuItem("Save All"))
				{
					AssetsManager::SaveAllAssets();
				}

				ImGui::EndMenu();				
			}

			dragZoneWidth -= ImGui::GetItemRectMax().x;

			// Drag zone
			//ImGui::Button("Drag zone", ImVec2(dragZoneWidth, titleBarHeight));
			ImGui::InvisibleButton("Drag zone", ImVec2(dragZoneWidth, titleBarHeight));
			isTitleBarHovered = ImGui::IsItemHovered();
			
			// Minimize Button
			ImGui::SameLine(titleBarWidth - 3.0f * titleBarHeight);
			if (ImGui::ImageButton(minimizeButtonTextureID, buttonsSize))
			{
				glfwIconifyWindow(window);
			}
			
			// Maximize Button
			ImGui::SameLine(titleBarWidth - 2.0f * titleBarHeight);
			if (ImGui::ImageButton(maximizeButtonTextureID, buttonsSize))
			{
				if (isWindowMaximized)
				{
					glfwRestoreWindow(window);
				}
				else
				{
					glfwMaximizeWindow(window);
				}

				isWindowMaximized = !isWindowMaximized;
			}
			
			// Close Button
			ImGui::SameLine(titleBarWidth - titleBarHeight);
			if (ImGui::ImageButton(closeButtonTextureID, buttonsSize))
			{
				glfwSetWindowShouldClose(window, true);
			}

			ImGui::EndMenuBar();
		}
	}

	bool EditorTitleBar::IsTitleBarHovered() const
	{
		return isTitleBarHovered;
	}

	void EditorTitleBar::InitWindow(GLFWwindow* window)
	{
		this->window = window;
	}
}