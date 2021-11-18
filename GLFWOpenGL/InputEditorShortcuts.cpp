#include "InputEditorShortcuts.h"

#include "InputManager.h"

void InputEditorShortcuts::ProcessShortcuts(GLFWwindow* window)
{
	if (InputManager::GetKeyReleased(GLFW_KEY_H))
	{
		InputManager::SetCursorEnabled(!InputManager::GetCursorEnabled());
	}
	
	if (InputManager::GetKeyReleased(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}