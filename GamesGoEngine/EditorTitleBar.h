#pragma once

#include <imgui.h>

#include "EditorPanel.h"

struct GLFWwindow;

namespace GamesGoEngine
{
	class EditorTitleBar : public EditorPanel
	{
	public:
		EditorTitleBar();
		~EditorTitleBar();

		virtual void Draw() override;
		bool IsTitleBarHovered() const;
		void InitWindow(GLFWwindow* window);

	private:
		GLFWwindow* window;
		bool isTitleBarHovered;
		bool isWindowMaximized;

		unsigned int minimizeButtonTexture;
		unsigned int maximizeButtonTexture;
		unsigned int closeButtonTexture;

		ImTextureID minimizeButtonTextureID;
		ImTextureID maximizeButtonTextureID;
		ImTextureID closeButtonTextureID;

		const ImVec2 buttonsSize = { 16.0f, 16.0f };
	};
}