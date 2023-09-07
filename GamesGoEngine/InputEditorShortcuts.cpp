#include "InputEditorShortcuts.h"

#include "EditorUIManager.h"
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

		if (InputManager::GetKeyReleased(GLFW_KEY_W))
		{
			EditorUIManager::SetTransformOperation(ImGuizmo::OPERATION::TRANSLATE);
		}

		if (InputManager::GetKeyReleased(GLFW_KEY_E))
		{
			EditorUIManager::SetTransformOperation(ImGuizmo::OPERATION::ROTATE);
		}

		if (InputManager::GetKeyReleased(GLFW_KEY_R))
		{
			EditorUIManager::SetTransformOperation(ImGuizmo::OPERATION::SCALE);
		}
	}
}