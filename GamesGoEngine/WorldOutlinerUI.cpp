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
			float objectIndentation;
			if (sceneObject->GetAllParentsNumber() > 0)
			{
				objectIndentation = (float)sceneObject->GetAllParentsNumber() * IndentSpace;
				ImGui::Indent(objectIndentation);
			}
			
			const bool selected = ImGui::Selectable(sceneObject->GetName().c_str(), selectedSceneObjectIndex == i);
			if (selected)
			{
				selectedSceneObjectIndex = (selectedSceneObjectIndex == i) ? -1 : i;
			}

			if (sceneObject->GetAllParentsNumber() > 0)
			{
				ImGui::Unindent(objectIndentation);
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