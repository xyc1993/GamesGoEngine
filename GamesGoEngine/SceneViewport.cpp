#include "SceneViewport.h"

#include <imgui.h>
#include "ImGuizmo.h"

#include "RenderingManager.h"
#include "CamerasManager.h"
#include "GameObject.h"

namespace GamesGoEngine
{
	void SceneViewport::Draw(GameObject* selectedSceneObject, float& viewportPosX, float& viewportPosY)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("Viewport", nullptr, windowFlags);

		// Using a Child allow to fill all the space of the window.
		ImGui::BeginChild("Viewport Image");
		
		// Get image handle and render it in this panel
		uint32_t textureId = RenderingManager::GetFinalColorBuffer();
		ImGui::Image((void*)textureId, ImGui::GetWindowSize(), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

		// Write current viewport position
		viewportPosX = ImGui::GetItemRectMin().x;
		viewportPosY = ImGui::GetItemRectMin().y;
		
		// Transform Gizmos
		if (selectedSceneObject != nullptr)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

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

		ImGui::EndChild();

		ImGui::End();
	}
}