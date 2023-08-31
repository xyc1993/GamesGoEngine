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
			unsigned int objectId = RenderingManager::GetObjectIdAt(x, y);
			
			GameObject* selectedGameObject = activeScene->GetGameObjectWithId(objectId);

			if (selectedGameObject != nullptr)
			{
				// if there was other game object that was selected, deselect it
				GameObject* previouslySelectedGameObject = activeScene->GetSelectedGameObject();
				if (previouslySelectedGameObject != nullptr)
				{
					previouslySelectedGameObject->SetSelected(false);
				}

				selectedGameObject->SetSelected(true);
			}
		}
		else
		{
			throw std::runtime_error("There's no active scene to pick game object from!");
		}
	}
}
