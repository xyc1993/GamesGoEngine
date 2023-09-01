#include "InputEditorShortcuts.h"

#include "imgui.h"
#include "InputManager.h"
#include "Scene.h"

namespace GamesGoEngine
{
	void InputEditorShortcuts::ProcessShortcuts(GLFWwindow* window, Scene* activeScene)
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

		if (InputManager::GetKeyReleased(GLFW_KEY_DELETE) && !ImGui::GetIO().WantTextInput)
		{
			GameObject* selectedGameObject = activeScene->GetSelectedGameObject();
			if (selectedGameObject != nullptr)
			{
				activeScene->RemoveGameObject(selectedGameObject);
			}
		}
	}
}