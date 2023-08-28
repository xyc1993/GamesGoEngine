#include "InputEditorShortcuts.h"

#include "InputManager.h"

namespace GamesGoEngine
{
	void InputEditorShortcuts::ProcessShortcuts(GLFWwindow* window)
	{
		if (InputManager::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			InputManager::SetCursorEnabled(false);
		}

		if (InputManager::GetMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT))
		{
			InputManager::SetCursorEnabled(true);
		}

		if (InputManager::GetKeyReleased(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}