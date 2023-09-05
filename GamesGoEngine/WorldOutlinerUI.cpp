#include "WorldOutlinerUI.h"

#include "imgui.h"
#include "InputManager.h"

namespace GamesGoEngine
{
	GameObject* WorldOutlinerUI::selectedSceneObject = nullptr;

	GameObject* WorldOutlinerUI::Draw(Scene* activeScene)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("World Outliner", nullptr, windowFlags);

		const std::set<GameObject*>& sceneObjects = activeScene->GetSceneObjects();

		// Check if the selected game object wasn't deselected via different means than this panel
		if (selectedSceneObject != nullptr && activeScene->GetSelectedGameObject() == nullptr)
		{
			selectedSceneObject = nullptr;
		}

		// Update selection if object was selected via different means (for example via clicking directly through the camera view)
		for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			GameObject* sceneObject = *it;
			if (sceneObject->IsSelected())
			{
				selectedSceneObject = sceneObject;
				break;
			}
		}

		// Perform standard UI selection
		for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			GameObject* sceneObject = *it;
			if (sceneObject != nullptr)
			{
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

				if (opened)
				{
					DrawSceneNodeChildren(sceneObject);
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

	void WorldOutlinerUI::DrawSceneNodeChildren(GameObject* sceneObject)
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

				if (opened)
				{
					DrawSceneNodeChildren(child);
				}
			}
		}
		ImGui::TreePop();
	}
}