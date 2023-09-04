#include "SceneViewport.h"

#include <imgui.h>
#include "ImGuizmo.h"

#include "RenderingManager.h"
#include "CamerasManager.h"
#include "GameObject.h"

namespace GamesGoEngine
{
	void SceneViewport::Draw(float width, float height, GameObject* selectedSceneObject)
	{
		ImGui::Begin("Viewport");

		uint32_t textureId = RenderingManager::GetFinalColorBuffer();
		ImGui::Image((void*)textureId, ImVec2(width, height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

		// Transform Gizmos
		if (selectedSceneObject != nullptr)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			const float windowWidth = (float)ImGui::GetWindowWidth();
			const float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Get camera data
			const glm::mat4 cameraView = CamerasManager::GetActiveCameraViewMatrix();
			const glm::mat4 cameraProjection = CamerasManager::GetActiveCameraProjectionMatrix();

			// Get selected object data
			glm::mat4 selectedObjectTransform = selectedSceneObject->GetTransform()->GetTransformMatrix();

			// Draw gizmo
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, glm::value_ptr(selectedObjectTransform));

			if (ImGuizmo::IsUsing())
			{
				selectedSceneObject->GetTransform()->SetPosition(glm::vec3(selectedObjectTransform[3]));
			}
		}

		ImGui::End();
	}
}