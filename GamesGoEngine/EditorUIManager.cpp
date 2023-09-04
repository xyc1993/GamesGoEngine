#include "EditorUIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ImGuizmo.h>

#include "LoggerUI.h"
#include "PropertiesUI.h"
#include "WorldOutlinerUI.h"
#include "DebugToolsUI.h"
#include "GraphicsSettingsUI.h"
#include "SceneViewport.h"

namespace GamesGoEngine
{
	float EditorUIManager::windowWidth = 0.0f;
	float EditorUIManager::windowHeight = 0.0f;
	float EditorUIManager::viewportPanelWidth = 0.0f;
	float EditorUIManager::viewportPanelHeight = 0.0f;
	float EditorUIManager::viewportTextureWidth = 0.0f;
	float EditorUIManager::viewportTextureHeight = 0.0f;

	void EditorUIManager::Init(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		UpdateViewportDimensions();
	}

	void EditorUIManager::Draw(Scene* activeScene)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		const float uiScale = GetUIScale();

		ImGui::GetIO().FontGlobalScale = uiScale;

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(0.125f * windowWidth, 0.33f * windowHeight));

		GameObject* selectedSceneObject = WorldOutlinerUI::Draw(activeScene);

		ImGui::SetNextWindowPos(ImVec2(0.8125f * windowWidth, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(0.1875f * windowWidth, 0.33f * windowHeight));

		PropertiesUI::Draw(selectedSceneObject);

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.67f * windowHeight));
		ImGui::SetNextWindowSize(ImVec2(0.25f * windowWidth, 0.33f * windowHeight));

		GraphicsSettingsUI::Draw();

		ImGui::SetNextWindowPos(ImVec2(0.25f * windowWidth, 0.67f * windowHeight));
		ImGui::SetNextWindowSize(ImVec2(0.5f * windowWidth, 0.33f * windowHeight));

		LoggerUI::Draw();

		ImGui::SetNextWindowPos(ImVec2(0.75f * windowWidth, 0.67f * windowHeight));
		ImGui::SetNextWindowSize(ImVec2(0.25f * windowWidth, 0.33f * windowHeight));

		DebugToolsUI::Draw();

		UpdateViewportDimensions();
		ImGui::SetNextWindowPos(ImVec2(0.125f * windowWidth, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(viewportPanelWidth, viewportPanelHeight));

		SceneViewport::Draw(viewportTextureWidth, viewportTextureHeight, selectedSceneObject);
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorUIManager::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	float EditorUIManager::GetViewportWidth()
	{
		return viewportTextureWidth;
	}

	float EditorUIManager::GetViewportHeight()
	{
		return viewportTextureHeight;
	}

	void EditorUIManager::UpdateViewportDimensions()
	{
		viewportPanelWidth = 0.8125f * windowWidth - 0.125f * windowWidth;
		viewportPanelHeight = 0.67f * windowHeight;

		viewportTextureWidth = viewportPanelWidth;
		viewportTextureHeight = viewportPanelHeight - 40.0f; // 40 pixels is reserved for label padding
	}

	void EditorUIManager::UpdateWindowSize(float width, float height)
	{
		windowWidth = width;
		windowHeight = height;
	}

	float EditorUIManager::GetUIScale()
	{
		// Calculate scale factor based on the following values with assuming linear scaling (based on trial and error)
		// 3840 x 2160 :: 3.0

		if (windowWidth / 16.0f > windowHeight / 9.0f) // aspect ratio wider than 16:9, scale to lower density value of height
		{
			return 3.0f * windowHeight / 2160.0f;
		}
		else // aspect ratio narrower than 16:9, scale to lower density value of width
		{
			return 3.0f * windowWidth / 3840.0f;
		}
	}
}