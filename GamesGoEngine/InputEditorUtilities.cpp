#include "InputEditorUtilities.h"

#include "RenderingManager.h"
#include "EditorUIManager.h"
#include "SceneManager.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace GamesGoEngine
{
	void InputEditorUtilities::SelectGameObjectAt(int x, int y)
	{
		// If transform gizmos are used, don't try to select an object
		if (ImGuizmo::IsOver())
		{
			return;
		}

		// Transform window coordinates to viewport coordinates
		const int viewportPosX = static_cast<int>(EditorUIManager::GetViewportPosX());
		const int viewportPosY = static_cast<int>(EditorUIManager::GetViewportPosY());
		const int viewportWidth = static_cast<int>(EditorUIManager::GetViewportWidth());
		const int viewportHeight = static_cast<int>(EditorUIManager::GetViewportHeight());
		const int viewportX = x - viewportPosX;
		const int viewportY = viewportHeight - (y - viewportPosY); // We need to revert Y, max value at the top not bottom

		// Check if it's within bounds
		if (viewportX < 0 || viewportX > viewportWidth || viewportY < 0 || viewportY > viewportHeight)
		{
			return;
		}

		if (Scene* activeScene = SceneManager::GetActiveScene())
		{
			// If there was other game object that was selected, deselect it
			// We want to always deselect the current object in order to have an option to deselect items by pressing on the void space
			GameObject* previouslySelectedGameObject = activeScene->GetSelectedGameObject();
			if (previouslySelectedGameObject != nullptr)
			{
				previouslySelectedGameObject->SetSelected(false);
			}

			// Get the id and try to find a matching game object, select it if found
			const int objectId = RenderingManager::GetObjectIdAt(viewportX, viewportY);
			GameObject* selectedGameObject = activeScene->GetGameObjectWithId(objectId);
			if (selectedGameObject != nullptr)
			{
				selectedGameObject->SetSelected(true);
			}
		}
		else
		{
			throw std::runtime_error("There's no active scene to pick game object from!");
		}
	}
}
