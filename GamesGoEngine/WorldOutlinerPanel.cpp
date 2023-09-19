#include "WorldOutlinerPanel.h"

#include <imgui.h>

#include "InputManager.h"
#include "SceneManager.h"

namespace GamesGoEngine
{
	WorldOutlinerPanel::WorldOutlinerPanel()
	{

	}

	void WorldOutlinerPanel::Draw()
	{
		Scene* activeScene = SceneManager::GetActiveScene();
		if (activeScene == nullptr)
		{
			return;
		}

		GameObject* selectedGameObject = activeScene->GetSelectedGameObject();
		
		ImGui::Begin("World Outliner");

		const std::map<int, GameObject*>& sceneObjects = activeScene->GetSceneObjects();

		// Check if the selected game object wasn't deselected via different means than this panel
		if (selectedGameObject != nullptr && activeScene->GetSelectedGameObject() == nullptr)
		{
			selectedGameObject = nullptr;
		}

		// Update selection if object was selected via different means (for example via clicking directly through the camera view)
		for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			GameObject* sceneObject = it->second;
			if (sceneObject->IsSelected())
			{
				selectedGameObject = sceneObject;
				break;
			}
		}

		// Perform standard UI selection
		for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			GameObject* sceneObject = it->second;
			if (sceneObject != nullptr)
			{
				// Game objects are processed down the hierarchy, never up, ignore children
				if (sceneObject->GetAllParentsNumber() > 0)
				{
					continue;
				}

				ImGuiTreeNodeFlags flags = (selectedGameObject == sceneObject ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				const bool opened = ImGui::TreeNodeEx((void*)(uint64_t)sceneObject, flags, sceneObject->GetName().c_str());

				if (ImGui::IsItemClicked())
				{
					if (selectedGameObject != nullptr)
					{
						selectedGameObject->SetSelected(false);
					}
					selectedGameObject = sceneObject;
					selectedGameObject->SetSelected(true);
				}

				HandleDragAndDrop(activeScene, sceneObject);

				if (opened)
				{
					DrawSceneNodeChildren(activeScene, sceneObject);
				}
			}
			else
			{
				ImGui::Text("DELETED");
			}
		}

		ImGui::End();
	}

	void WorldOutlinerPanel::DrawSceneNodeChildren(Scene* activeScene, GameObject* sceneObject)
	{
		GameObject* selectedGameObject = activeScene->GetSelectedGameObject();
		for (int j = 0; j < sceneObject->GetChildren().size(); j++)
		{
			GameObject* child = sceneObject->GetChildren()[j];
			if (child != nullptr)
			{
				ImGuiTreeNodeFlags flags = (selectedGameObject == child ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)child, flags, child->GetName().c_str());

				if (ImGui::IsItemClicked())
				{
					if (selectedGameObject != nullptr)
					{
						selectedGameObject->SetSelected(false);
					}
					selectedGameObject = child;
					selectedGameObject->SetSelected(true);
				}

				HandleDragAndDrop(activeScene, child);

				if (opened)
				{
					DrawSceneNodeChildren(activeScene, child);
				}
			}
		}
		ImGui::TreePop();
	}

	void WorldOutlinerPanel::HandleDragAndDrop(Scene* activeScene, GameObject* sceneObject)
	{
		if (ImGui::BeginDragDropSource())
		{
			int gameObjectId = sceneObject->GetObjectId();
			ImGui::SetDragDropPayload("SceneHierarchy", &gameObjectId, sizeof(int));
			ImGui::Text(sceneObject->GetName().c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneHierarchy"))
			{
				int dragDropGameObjectId = *(int*)payload->Data;
				GameObject* droppedGameObject = activeScene->GetGameObjectWithId(dragDropGameObjectId);
				if (droppedGameObject)
				{
					droppedGameObject->SetParent(sceneObject);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}