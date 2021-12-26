#include "WorldOutlinerUI.h"

#include "imgui.h"

GameObject* WorldOutlinerUI::selectedSceneObject = nullptr;

GameObject* WorldOutlinerUI::Draw(const Scene& activeScene)
{
	ImGui::Begin("World Outliner");
	for (int i = 0; i < activeScene.GetSceneObjects().size(); i++)
	{
		GameObject* sceneObject = activeScene.GetSceneObjects()[i];
		if (sceneObject != nullptr)
		{
			if (sceneObject->GetAllParentsNumber() > 0)
			{
				continue;
			}

			ImGuiTreeNodeFlags flags = (selectedSceneObject == sceneObject ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)sceneObject, flags, sceneObject->GetName().c_str());

			if (ImGui::IsItemClicked())
			{
				selectedSceneObject = sceneObject;
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
				selectedSceneObject = child;
			}

			if (opened)
			{
				DrawSceneNodeChildren(child);
			}
		}
	}
	ImGui::TreePop();
}