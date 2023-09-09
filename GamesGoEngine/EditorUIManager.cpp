#include "EditorUIManager.h"

#include "WindowManager.h"

#include <GLFW/glfw3.h>
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
	EditorUIManager* EditorUIManager::instance = nullptr;

	EditorUIManager::EditorUIManager()
	{
		debugToolsUI = new DebugToolsUI();
		graphicsSettingsUI = new GraphicsSettingsUI();
		loggerUI = new LoggerUI();
		propertiesUI = new PropertiesUI();
		sceneViewer = new SceneViewport();
		worldOutliner = new WorldOutlinerUI();

		// set panels sizes and position offsets
		debugToolsUI->SetDrawingMultipliers(0.75f, 0.67f, 0.25f, 0.33f);
		graphicsSettingsUI->SetDrawingMultipliers(0.0f, 0.67f, 0.25f, 0.33f);
		loggerUI->SetDrawingMultipliers(0.25f, 0.67f, 0.5f, 0.33f);
		propertiesUI->SetDrawingMultipliers(0.8125f, 0.0f, 0.1875f, 0.33f);
		sceneViewer->SetDrawingMultipliers(0.125f, 0.0f, 0.6875f, 0.67f);
		worldOutliner->SetDrawingMultipliers(0.0f, 0.0f, 0.125f, 0.33f);

		// fill editor panels container
		editorPanels.push_back(debugToolsUI);
		editorPanels.push_back(graphicsSettingsUI);
		editorPanels.push_back(loggerUI);
		editorPanels.push_back(propertiesUI);
		editorPanels.push_back(sceneViewer);
		editorPanels.push_back(worldOutliner);
	}

	EditorUIManager::~EditorUIManager()
	{
		delete debugToolsUI;
		delete graphicsSettingsUI;
		delete loggerUI;
		delete propertiesUI;
		delete sceneViewer;
		delete worldOutliner;

		editorPanels.clear();
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
	}

	void EditorUIManager::Draw()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		const float windowWidth = WindowManager::GetCurrentWidth();
		const float windowHeight = WindowManager::GetCurrentHeight();
		
		const float uiScale = GetUIScale(windowWidth, windowHeight);

		ImGui::GetIO().FontGlobalScale = uiScale;
		
		GetInstance()->DrawPanels(windowWidth, windowHeight);
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorUIManager::DrawPanels(const float windowWidth, const float windowHeight)
	{
		for (size_t i = 0; i < editorPanels.size(); i++)
		{
			editorPanels[i]->DrawToWindowDimensions(windowWidth, windowHeight);
		}
	}

	void EditorUIManager::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorUIManager::SetTransformOperation(ImGuizmo::OPERATION transformOperation)
	{
		GetInstance()->sceneViewer->SetTransformOperation(transformOperation);
	}

	void EditorUIManager::SelectGameObjectAt(int x, int y)
	{
		GetInstance()->sceneViewer->SelectGameObjectAt(x, y);
	}

	void EditorUIManager::AddLog(MsgType type, std::string msg)
	{
		GetInstance()->loggerUI->AddLog(type, msg);
	}

	void EditorUIManager::ClearLogs()
	{
		GetInstance()->loggerUI->ClearLogs();
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