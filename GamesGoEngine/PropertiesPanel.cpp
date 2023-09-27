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
			DrawComponentsFields(selectedGameObject);
			DrawAddComponentButton(selectedGameObject);
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

	void PropertiesPanel::DrawComponentsFields(GameObject* selectedGameObject)
	{
		const std::vector<Component*> components = selectedGameObject->GetComponents();
		const size_t componentsNumber = components.size();
		for (int i = 0; i < componentsNumber; i++)
		{
			Component* component = components[i];
			const ClassMetaData metaData = component->GetMetaData();

			bool removedComponent = false;
			if (!metaData.className.empty())
			{
				// Label
				ImGui::Text(metaData.className.c_str());
				// Settings such as "Remove component"
				ImGui::SameLine();

				/*
				 * When displaying a list of objects, using indices or pointers as ID will preserve the node open/closed state per object
				 * When passing a label you can optionally specify extra unique ID information within the same string using "##".
				 * This helps solving the simpler collision cases. e.g. "Label##Foobar" display "Label" and uses "Label##Foobar" as ID
				 */
				std::string buttonLabel = "+##";
				buttonLabel.append(std::to_string(i));

				std::string componentSettingsId = "ComponentSettings";
				componentSettingsId.append(std::to_string(i));
				
				if (ImGui::Button(buttonLabel.c_str()))
				{
					ImGui::OpenPopup(componentSettingsId.c_str());
				}
				
				if (ImGui::BeginPopup(componentSettingsId.c_str()))
				{
					if (ImGui::MenuItem("Remove component"))
					{
						selectedGameObject->RemoveComponent(component);
						removedComponent = true;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			if (removedComponent)
			{
				break;
			}

			if (!metaData.classFields.empty())
			{
				for (size_t j = 0; j < metaData.classFields.size(); j++)
				{
					Field field = metaData.classFields[j];

					// Append "##"ij to avoid label collision where there are 2 fields with the same name
					std::string fieldLabel = field.fieldName;
					fieldLabel.append("##");
					fieldLabel.append(std::to_string(i));
					fieldLabel.append(std::to_string(j));

					if (field.typeName == "bool")
					{
						bool* boolField = static_cast<bool*>(field.fieldPointer);
						if (ImGui::Checkbox(fieldLabel.c_str(), boolField))
						{

						}
					}

					if (field.typeName == "float")
					{
						float* floatField = static_cast<float*>(field.fieldPointer);
						if (ImGui::DragFloat(fieldLabel.c_str(), floatField, 0.1f))
						{

						}
					}

					if (field.typeName == "int")
					{
						int* intField = static_cast<int*>(field.fieldPointer);
						if (ImGui::DragInt(fieldLabel.c_str(), intField, 1))
						{

						}
					}

					if (field.typeName == "glm::vec3")
					{
						glm::vec3* vec3Field = static_cast<glm::vec3*>(field.fieldPointer);
						if (ImGui::DragFloat3(fieldLabel.c_str(), glm::value_ptr(*vec3Field), 0.1f))
						{

						}
					}
				}
			}
		}
	}

	void PropertiesPanel::DrawAddComponentButton(GameObject* selectedGameObject)
	{
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			auto registeredComponents = ReflectionFactory::GetReflectableObjectsFactory()->classes;
			for (auto it = registeredComponents.begin(); it != registeredComponents.end(); ++it)
			{
				std::string componentName = it->first;
				if (ImGui::MenuItem(componentName.c_str()))
				{
					Component* component = (Component*)ReflectionFactory::GetReflectableObjectsFactory()->Construct(componentName);
					selectedGameObject->AddComponent(component);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}
}