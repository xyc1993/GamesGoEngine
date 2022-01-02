#include "PropertiesUI.h"

#include "imgui.h"

void PropertiesUI::Draw(GameObject* selectedGameObject)
{
	ImGui::Begin("Properties Panel");

	if (selectedGameObject != nullptr)
	{
		DrawNameInputField(selectedGameObject);
		DrawTransformField(selectedGameObject);
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

void PropertiesUI::DrawTransformField(GameObject* selectedGameObject)
{
	ImGui::Text("Transform");

	Transform* transform = selectedGameObject->GetTransform();

	glm::vec3 localPosition = transform->GetLocalPosition();
	if (ImGui::DragFloat3("Position", glm::value_ptr(localPosition), 0.1f))
	{
		transform->SetLocalPosition(localPosition);
	}

	const glm::vec3 oldLocalRotation = transform->GetLocalRotationEulerDegrees();
	glm::vec3 localRotation = oldLocalRotation;
	if (ImGui::DragFloat3("Rotation", glm::value_ptr(localRotation), 0.1f))
	{
		const glm::quat rotationDifference = glm::quat(localRotation - oldLocalRotation);
		transform->SetLocalRotation(rotationDifference * transform->GetLocalRotation());
	}

	glm::vec3 localScale = transform->GetLocalScale();
	if (ImGui::DragFloat3("Scale", glm::value_ptr(localScale), 0.1f))
	{
		transform->SetLocalScale(localScale);
	}
}