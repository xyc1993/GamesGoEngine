#include "WorldOutlinerUI.h"

#include "imgui.h"
#include "InputManager.h"

namespace GamesGoEngine
{
	GameObject* WorldOutlinerUI::selectedSceneObject = nullptr;

	GameObject* WorldOutlinerUI::Draw(Scene* activeScene)
	{
		ImGui::Begin("World Outliner");

		const std::set<GameObject*>& sceneObjects = activeScene->GetSceneObjects();

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

		if (ImGui::IsWindowFocused())
		{
			if (InputManager::GetKeyReleased(GLFW_KEY_DELETE))
			{
				activeScene->RemoveGameObject(selectedSceneObject);
				selectedSceneObject = nullptr;
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