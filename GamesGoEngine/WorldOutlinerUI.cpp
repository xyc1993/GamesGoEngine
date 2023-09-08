#include "WorldOutlinerUI.h"

#include "imgui.h"
#include "InputManager.h"
#include "SceneManager.h"

namespace GamesGoEngine
{
	GameObject* WorldOutlinerUI::selectedSceneObject = nullptr;

	GameObject* WorldOutlinerUI::Draw()
	{
		Scene* activeScene = SceneManager::GetActiveScene();
		if (activeScene == nullptr)
		{
			return nullptr;
		}

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("World Outliner", nullptr, windowFlags);

		const std::map<int, GameObject*>& sceneObjects = activeScene->GetSceneObjects();

		// Check if the selected game object wasn't deselected via different means than this panel
		if (selectedSceneObject != nullptr && activeScene->GetSelectedGameObject() == nullptr)
		{
			selectedSceneObject = nullptr;
		}

		// Update selection if object was selected via different means (for example via clicking directly through the camera view)
		for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			GameObject* sceneObject = it->second;
			if (sceneObject->IsSelected())
			{
				selectedSceneObject = sceneObject;
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

				ImGuiTreeNodeFlags flags = (selectedSceneObject == sceneObject ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				const bool opened = ImGui::TreeNodeEx((void*)(uint64_t)sceneObject, flags, sceneObject->GetName().c_str());

				if (ImGui::IsItemClicked())
				{
					if (selectedSceneObject != nullptr)
					{
						selectedSceneObject->SetSelected(false);
					}
					selectedSceneObject = sceneObject;
					selectedSceneObject->SetSelected(true);
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

		return selectedSceneObject;
	}

	void WorldOutlinerUI::DrawSceneNodeChildren(Scene* activeScene, GameObject* sceneObject)
	{
		for (int j = 0; j < sceneObject->GetChildren().size(); j++)
		{
			GameObject* child = sceneObject->GetChildren()[j];
			if (child != nullptr)
			{
				ImGuiTreeNodeFlags flags = (selectedSceneObject == child ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)child, flags, child->GetName().c_str());

				if (ImGui::IsItemClicked())
				{
					if (selectedSceneObject != nullptr)
					{
						selectedSceneObject->SetSelected(false);
					}
					selectedSceneObject = child;
					selectedSceneObject->SetSelected(true);
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

	void WorldOutlinerUI::HandleDragAndDrop(Scene* activeScene, GameObject* sceneObject)
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