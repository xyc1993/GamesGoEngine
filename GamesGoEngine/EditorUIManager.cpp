#include "EditorUIManager.h"

#include "LoggerUI.h"
#include "PropertiesUI.h"
#include "WorldOutlinerUI.h"
#include "DebugToolsUI.h"
#include "GraphicsSettingsUI.h"
#include "SceneViewport.h"

namespace GamesGoEngine
{
	EditorUIManager* EditorUIManager::instance = nullptr;

	EditorUIManager::EditorUIManager()
	{
	}

	EditorUIManager* EditorUIManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new EditorUIManager();
		}
		return instance;
	}

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

		const float windowWidth = GetInstance()->windowWidth;
		const float windowHeight = GetInstance()->windowHeight;
		const float uiScale = GetUIScale(windowWidth, windowHeight);

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
		ImGui::SetNextWindowSize(ImVec2(GetInstance()->viewportPanelWidth, GetInstance()->viewportPanelHeight));

		SceneViewport::Draw(selectedSceneObject, GetInstance()->currentTransformOperation, GetInstance()->viewportPanelPosX, GetInstance()->viewportPanelPosY);
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorUIManager::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	float EditorUIManager::GetViewportPosX()
	{
		return GetInstance()->viewportPanelPosX;
	}

	float EditorUIManager::GetViewportPosY()
	{
		return GetInstance()->viewportPanelPosY;
	}

	float EditorUIManager::GetViewportWidth()
	{
		return GetInstance()->viewportTextureWidth;
	}

	float EditorUIManager::GetViewportHeight()
	{
		return GetInstance()->viewportTextureHeight;
	}

	void EditorUIManager::UpdateViewportDimensions()
	{
		GetInstance()->viewportPanelWidth = 0.8125f * GetInstance()->windowWidth - 0.125f * GetInstance()->windowWidth;
		GetInstance()->viewportPanelHeight = 0.67f * GetInstance()->windowHeight;

		// Padding comes from how ImGui dedicates space for child that fills the panel (plus label on top if enabled)
		GetInstance()->viewportTextureWidth = GetInstance()->viewportPanelWidth - 16.0f; // 16 pixels are for padding on the edges
		GetInstance()->viewportTextureHeight = GetInstance()->viewportPanelHeight - 30.0f - 9.0f; // 30 pixels are reserved for label padding and 9 are reserved for border padding
	}

	void EditorUIManager::UpdateWindowSize(float width, float height)
	{
		GetInstance()->windowWidth = width;
		GetInstance()->windowHeight = height;
	}

	void EditorUIManager::SetTransformOperation(ImGuizmo::OPERATION transformOperation)
	{
		GetInstance()->currentTransformOperation = transformOperation;
	}

	float EditorUIManager::GetUIScale(const float& windowWidth, const float& windowHeight)
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