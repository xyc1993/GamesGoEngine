#include "InputEditorUtilities.h"

#include "RenderingManager.h"
#include "SceneManager.h"
#include "imgui.h"

namespace GamesGoEngine
{
	void InputEditorUtilities::SelectGameObjectAt(int x, int y)
	{
		// If ImGui wants to capture mouse (we hover over some ImGui panel), don't try to select a game object
		if (ImGui::GetIO().WantCaptureMouse)
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
			const int objectId = RenderingManager::GetObjectIdAt(x, y);			
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
