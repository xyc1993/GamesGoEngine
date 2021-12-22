#include "InputEditorShortcuts.h"

#include "InputManager.h"

void InputEditorShortcuts::ProcessShortcuts(GLFWwindow* window, Scene* activeScene, int selectedGameObjectIndex)
{
	if (InputManager::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		InputManager::SetCursorEnabled(false);
	}

	if (InputManager::GetMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT))
	{
		InputManager::SetCursorEnabled(true);
	}

	if (InputManager::GetKeyReleased(GLFW_KEY_DELETE))
	{
		activeScene->RemoveGameObject(selectedGameObjectIndex);
	}
	
	if (InputManager::GetKeyReleased(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}