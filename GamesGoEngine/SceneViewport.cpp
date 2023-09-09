#include "SceneViewport.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "RenderingManager.h"
#include "CamerasManager.h"
#include "GameObject.h"
#include "SceneManager.h"

namespace GamesGoEngine
{
	SceneViewport::SceneViewport()
	{
		currentTransformOperation = ImGuizmo::OPERATION::TRANSLATE;
		viewportPanelPosX = 0;
		viewportPanelPosY = 0;
		viewportTextureWidth = 800;
		viewportTextureHeight = 450;
	}

	void SceneViewport::Draw()
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
		viewportPanelPosX = static_cast<int>(ImGui::GetItemRectMin().x);
		viewportPanelPosY = static_cast<int>(ImGui::GetItemRectMin().y);
		const int newViewportWidth = static_cast<int>(ImGui::GetItemRectSize().x);
		const int newViewportHeight = static_cast<int>(ImGui::GetItemRectSize().y);

		// if one of the viewport dimensions needs update, request also framebuffers update
		if (newViewportWidth != viewportTextureWidth || newViewportHeight != viewportTextureHeight)
		{
			viewportTextureWidth = newViewportWidth;
			viewportTextureHeight = newViewportHeight;
			RenderingManager::ResizeBuffers(viewportTextureWidth, viewportTextureHeight);
		}
		
		// Transform Gizmos
		GameObject* selectedGameObject = nullptr;
		if (Scene* activeScene = SceneManager::GetActiveScene())
		{
			selectedGameObject = activeScene->GetSelectedGameObject();
		}

		if (selectedGameObject != nullptr)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

			// Get camera data
			const glm::mat4 cameraView = CamerasManager::GetActiveCameraViewMatrix();
			const glm::mat4 cameraProjection = CamerasManager::GetActiveCameraProjectionMatrix();

			// Get selected object data
			glm::mat4 selectedObjectTransformMatrix = selectedGameObject->GetTransform()->GetTransformMatrix();

			// Draw gizmo
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				currentTransformOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr(selectedObjectTransformMatrix));

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
					selectedGameObject->GetTransform()->SetPosition(translation);
					selectedGameObject->GetTransform()->SetRotation(orientation);
					selectedGameObject->GetTransform()->SetScale(scale);
				}
			}
		}

		ImGui::EndChild();

		ImGui::End();
	}

	void SceneViewport::SetTransformOperation(ImGuizmo::OPERATION transformOperation)
	{
		currentTransformOperation = transformOperation;
	}

	void SceneViewport::SelectGameObjectAt(int x, int y)
	{
		// If transform gizmos are used, don't try to select an object
		if (ImGuizmo::IsOver())
		{
			return;
		}

		// Transform window coordinates to viewport coordinates
		const int viewportX = x - viewportPanelPosX;
		const int viewportY = viewportTextureHeight - (y - viewportPanelPosY); // We need to revert Y, max value at the top not bottom

		// Check if it's within bounds
		if (viewportX < 0 || viewportX > viewportTextureWidth || viewportY < 0 || viewportY > viewportTextureHeight)
		{
			return;
		}

		if (Scene* activeScene = SceneManager::GetActiveScene())
		{
			// If there was other game object that was selected, deselect it
			// We want to always deselect the current object in order to have an option to deselect items by pressing on the void space
			GameObject* previouslySelectedGameObject = activeScene->GetSelectedGameObject();
			if (previouslySelectedGameObject != nullptr)
			{
				previouslySelectedGameObject->SetSelected(false);
			}

			// Get the id and try to find a matching game object, select it if found
			const int objectId = RenderingManager::GetObjectIdAt(viewportX, viewportY);
			GameObject* selectedGameObject = activeScene->GetGameObjectWithId(objectId);
			if (selectedGameObject != nullptr)
			{
				selectedGameObject->SetSelected(true);
			}
		}
		else
		{
			throw std::runtime_error("There's no active scene to pick game object from!");
		}
	}
}