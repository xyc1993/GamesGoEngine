#include "PropertiesUI.h"

#include "imgui.h"

void PropertiesUI::Draw(GameObject* selectedGameObject)
{
	ImGui::Begin("Properties Panel");

	if (selectedGameObject != nullptr)
	{
		DrawNameInputField(selectedGameObject);
	}

	ImGui::End();
}

void PropertiesUI::DrawNameInputField(GameObject* selectedGameObject)
{
	const std::string nameString = selectedGameObject->GetName();
	
	static char nameBuffer[GameObject::NAME_MAX_LENGTH];
	strcpy_s(nameBuffer, sizeof(nameBuffer), nameString.c_str());

	if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
	{
		selectedGameObject->SetName(nameBuffer);
	}
}