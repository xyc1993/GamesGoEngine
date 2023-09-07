#include "SceneViewport.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "RenderingManager.h"
#include "CamerasManager.h"
#include "GameObject.h"

namespace GamesGoEngine
{
	void SceneViewport::Draw(GameObject* selectedSceneObject, ImGuizmo::OPERATION transformOperation, float& viewportPosX, float& viewportPosY)
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
			glm::mat4 selectedObjectTransformMatrix = selectedSceneObject->GetTransform()->GetTransformMatrix();

			// Draw gizmo
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				transformOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr(selectedObjectTransformMatrix));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation;
				glm::quat orientation;
				glm::vec3 scale;
				glm::vec3 skew;
				glm::vec4 perspective;

				// If transform matrix decomposition was successful then apply changes
				if (glm::decompose(selectedObjectTransformMatrix, scale, orientation, translation, skew, perspective))
				{
					selectedSceneObject->GetTransform()->SetPosition(translation);
					selectedSceneObject->GetTransform()->SetRotation(orientation);
					selectedSceneObject->GetTransform()->SetScale(scale);
				}
			}
		}

		ImGui::EndChild();

		ImGui::End();
	}
}