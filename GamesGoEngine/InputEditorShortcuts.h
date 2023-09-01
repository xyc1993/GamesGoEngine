#pragma once

#include <GLFW/glfw3.h>

namespace GamesGoEngine
{
	class Scene;

	class InputEditorShortcuts
	{
	public:
		static void ProcessShortcuts(GLFWwindow* window, Scene* activeScene);
	};
}