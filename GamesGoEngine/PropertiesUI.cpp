#include "PropertiesUI.h"

#include "imgui.h"

#include "GameObject.h"

namespace GamesGoEngine
{
	void PropertiesUI::Draw(GameObject* selectedGameObject)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("Properties Panel", nullptr, windowFlags);

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

		glm::vec3 localRotation = transform->GetHintLocalRotation();
		if (ImGui::DragFloat3("Rotation", glm::value_ptr(localRotation), 0.1f))
		{
			transform->SetHintLocalRotation(localRotation);
		}

		glm::vec3 localScale = transform->GetLocalScale();
		if (ImGui::DragFloat3("Scale", glm::value_ptr(localScale), 0.1f))
		{
			transform->SetLocalScale(localScale);
		}
	}
}