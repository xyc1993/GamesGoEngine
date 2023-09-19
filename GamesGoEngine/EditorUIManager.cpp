#include "EditorUIManager.h"

#include "WindowManager.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "DebugToolsUI.h"
#include "EditorSettingsPanel.h"
#include "GraphicsSettingsUI.h"
#include "LoggerUI.h"
#include "PropertiesUI.h"
#include "SceneViewport.h"
#include "WorldOutlinerUI.h"

namespace GamesGoEngine
{
	EditorUIManager* EditorUIManager::instance = nullptr;

	EditorUIManager::EditorUIManager()
	{
		debugToolsUI = new DebugToolsUI();
		editorSettingsPanel = new EditorSettingsPanel();
		graphicsSettingsUI = new GraphicsSettingsUI();
		loggerUI = new LoggerUI();
		propertiesUI = new PropertiesUI();
		sceneViewer = new SceneViewport();
		worldOutliner = new WorldOutlinerUI();

		// fill editor panels container
		editorPanels.push_back(debugToolsUI);
		editorPanels.push_back(editorSettingsPanel);
		editorPanels.push_back(graphicsSettingsUI);
		editorPanels.push_back(loggerUI);
		editorPanels.push_back(propertiesUI);
		editorPanels.push_back(sceneViewer);
		editorPanels.push_back(worldOutliner);
	}

	EditorUIManager::~EditorUIManager()
	{
		delete debugToolsUI;
		delete editorSettingsPanel;
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
		ImGui_ImplOpenGL3_Init("#version 440");
		io.ConfigFlags = ImGuiConfigFlags_None;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	void EditorUIManager::Draw()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		// Set dock space
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static bool p_open = true;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &p_open, window_flags);
		ImGui::PopStyleVar(3); // Pop all of the pushed styles

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// Menu bar
		/*
		if (ImGui::BeginMenuBar())
		{			
			if (ImGui::BeginMenu("File"))
			{
				// In the future this will be utilized for features such as applications options, file open/save, etc.
			}			
			ImGui::EndMenuBar();
		}
		*/

		// Draw individual panels
		GetInstance()->DrawPanels();

		ImGui::End();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorUIManager::DrawPanels()
	{
		for (size_t i = 0; i < editorPanels.size(); i++)
		{
			editorPanels[i]->Draw();
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
}