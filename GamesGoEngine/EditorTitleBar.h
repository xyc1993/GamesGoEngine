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

		ImTextureID minimizeButtonTexture;
		ImTextureID maximizeButtonTexture;
		ImTextureID closeButtonTexture;

		const ImVec2 buttonsSize = { 16.0f, 16.0f };
	};
}