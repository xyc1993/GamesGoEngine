#include "WorldOutlinerUI.h"

#include "imgui.h"

int WorldOutlinerUI::selectedSceneObjectIndex = -1;

int WorldOutlinerUI::Draw(const Scene& activeScene)
{
	ImGui::Begin("World Outliner");
	for (int i = 0; i < activeScene.GetSceneObjects().size(); i++)
	{
		const GameObject* sceneObject = activeScene.GetSceneObjects()[i];
		if (sceneObject != nullptr)
		{
			const bool selected = ImGui::Selectable(sceneObject->GetName().c_str(), selectedSceneObjectIndex == i);
			if (selected)
			{
				selectedSceneObjectIndex = (selectedSceneObjectIndex == i) ? -1 : i;
			}
		}
		else
		{
			ImGui::Text("DELETED");
		}
	}
	ImGui::End();

	return selectedSceneObjectIndex;
}