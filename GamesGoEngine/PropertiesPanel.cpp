#include "PropertiesPanel.h"

#include <imgui.h>

#include "GameObject.h"
#include "SceneManager.h"

namespace GamesGoEngine
{
	PropertiesPanel::PropertiesPanel()
	{

	}

	void PropertiesPanel::Draw()
	{
		ImGui::Begin("Properties Panel");

		GameObject* selectedGameObject = nullptr;
		if (Scene* activeScene = SceneManager::GetActiveScene())
		{
			selectedGameObject = activeScene->GetSelectedGameObject();
		}

		if (selectedGameObject != nullptr)
		{
			DrawNameInputField(selectedGameObject);
			DrawTransformField(selectedGameObject);
		}

		if (selectedGameObject != nullptr)
		{
			const std::vector<Component*> components = selectedGameObject->GetComponents();
			const size_t componentsNumber = components.size();
			for (int i = 0; i < componentsNumber; i++)
			{
				const Component* component = components[i];
				const ClassMetaData metaData = component->GetMetaData();
				ImGui::Text(metaData.className.c_str());

				if (!metaData.classFields.empty())
				{
					for (size_t j = 0; j < metaData.classFields.size(); j++)
					{
						Field field = metaData.classFields[j];

						if (field.typeName == "bool")
						{
							bool* boolField = static_cast<bool*>(field.fieldPointer);
							if (ImGui::Checkbox(field.fieldName.c_str(), boolField))
							{

							}
						}

						if (field.typeName == "float")
						{
							float* floatField = static_cast<float*>(field.fieldPointer);
							if (ImGui::DragFloat(field.fieldName.c_str(), floatField, 0.1f))
							{
								
							}
						}

						if (field.typeName == "int")
						{
							int* intField = static_cast<int*>(field.fieldPointer);
							if (ImGui::DragInt(field.fieldName.c_str(), intField, 1))
							{

							}
						}

						if (field.typeName == "glm::vec3")
						{
							glm::vec3* vec3Field = static_cast<glm::vec3*>(field.fieldPointer);
							if (ImGui::DragFloat3(field.fieldName.c_str(), glm::value_ptr(*vec3Field), 0.1f))
							{
								
							}
						}
					}
				}
			}
		}		

		ImGui::End();
	}

	void PropertiesPanel::DrawNameInputField(GameObject* selectedGameObject)
	{
		const std::string nameString = selectedGameObject->GetName();

		static char nameBuffer[GameObject::NAME_MAX_LENGTH];
		strcpy_s(nameBuffer, sizeof(nameBuffer), nameString.c_str());

		if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
		{
			selectedGameObject->SetName(nameBuffer);
		}
	}

	void PropertiesPanel::DrawTransformField(GameObject* selectedGameObject)
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
