#include "EditorTitleBar.h"

#include <imgui.h>

#include "TextureLoader.h"
#include "WindowManager.h"

namespace GamesGoEngine
{
	EditorTitleBar::EditorTitleBar()
	{
		isWindowMaximized = false;
		isTitleBarHovered = false;

		minimizeButtonTexture = reinterpret_cast<ImTextureID>(TextureLoader::LoadTexture(const_cast<GLchar*>("res/icons/minimize.png"), true, true));;
		maximizeButtonTexture = reinterpret_cast<ImTextureID>(TextureLoader::LoadTexture(const_cast<GLchar*>("res/icons/maximize.png"), true, true));;
		closeButtonTexture = reinterpret_cast<ImTextureID>(TextureLoader::LoadTexture(const_cast<GLchar*>("res/icons/close.png"), true, true));;
	}

	EditorTitleBar::~EditorTitleBar()
	{
		// TODO: textures should be deleted!
	}

	void EditorTitleBar::Draw()
	{
		if (ImGui::BeginMenuBar())
		{
			// TODO: add logo in top-right corner once it's done
			
			const float titleBarHeight = ImGui::GetFrameHeight() + 1.0f; // +1.0f to avoid button overlap
			const float titleBarWidth = ImGui::GetWindowSize().x;
			float dragZoneWidth = titleBarWidth - 3.0 * titleBarHeight - 10.0f; // -10.0f comes from padding; TODO: do it in more scalable way in case more buttons are added
			
			// In the future this will be utilized for features such as applications options, file open/save, etc.
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene"))
				{
					// Create and open a new, blank scene
				}

				if (ImGui::MenuItem("Open Scene"))
				{
					// Open existing scene from path
				}

				if (ImGui::MenuItem("Save Scene"))
				{
					// Save existing scene
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
			if (ImGui::ImageButton(minimizeButtonTexture, buttonsSize))
			{
				glfwIconifyWindow(window);
			}
			
			// Maximize Button
			ImGui::SameLine(titleBarWidth - 2.0f * titleBarHeight);
			if (ImGui::ImageButton(maximizeButtonTexture, buttonsSize))
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
			if (ImGui::ImageButton(closeButtonTexture, buttonsSize))
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