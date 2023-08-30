#include "InputEditorUtilities.h"

#include "RenderingManager.h"
#include "SceneManager.h"

namespace GamesGoEngine
{
	void InputEditorUtilities::SelectGameObjectAt(int x, int y)
	{
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
