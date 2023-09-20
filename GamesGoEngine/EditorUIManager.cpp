#include "EditorUIManager.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "DebugToolsPanel.h"
#include "EditorSettingsPanel.h"
#include "GraphicsSettingsPanel.h"
#include "LoggerPanel.h"
#include "PropertiesPanel.h"
#include "SceneViewportPanel.h"
#include "WorldOutlinerPanel.h"

namespace GamesGoEngine
{
	EditorUIManager* EditorUIManager::instance = nullptr;

	EditorUIManager::EditorUIManager()
	{
		debugToolsPanel = new DebugToolsPanel();
		editorSettingsPanel = new EditorSettingsPanel();
		graphicsSettingsPanel = new GraphicsSettingsPanel();
		loggerPanel = new LoggerPanel();
		propertiesPanel = new PropertiesPanel();
		sceneViewerPanel = new SceneViewportPanel();
		worldOutlinerPanel = new WorldOutlinerPanel();

		// fill editor panels container
		editorPanels.push_back(debugToolsPanel);
		editorPanels.push_back(editorSettingsPanel);
		editorPanels.push_back(graphicsSettingsPanel);
		editorPanels.push_back(loggerPanel);
		editorPanels.push_back(propertiesPanel);
		editorPanels.push_back(sceneViewerPanel);
		editorPanels.push_back(worldOutlinerPanel);
	}

	EditorUIManager::~EditorUIManager()
	{
		delete debugToolsPanel;
		delete editorSettingsPanel;
		delete graphicsSettingsPanel;
		delete loggerPanel;
		delete propertiesPanel;
		delete sceneViewerPanel;
		delete worldOutlinerPanel;

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
		ImGuiIO& io = ImGui::GetIO();

		// Set all of the used flags before init!
		io.ConfigFlags = ImGuiConfigFlags_None;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		/* TODO: proper viewports support (check notes on viewports here: https://ikrima.dev/dev-notes/imgui/imgui-multiviewport/), this includes:
		 * 1) Calculating proper position of viewport coordinates for object selection (with viewports pos is in relation to OS window)
		 * 2) Implementing proper input support for scene viewport when it's detached from main window (with viewports when detached it doesn't respond to current inputs)
		 * 3) Make sure ImGuizmo works correctly with detached viewport. Currently when viewport is detached, trying to move transform gizmos moves whole viewport.
		 */
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Load and set font
		io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/Poppins/Poppins-Regular.ttf", 17.0f);

		ImGui::StyleColorsDark();
		ImGui_ImplOpenGL3_Init("#version 440");
		ImGui_ImplGlfw_InitForOpenGL(window, true);
	}

	void EditorUIManager::Draw()
	{
		OnFrameDrawBegin();
		
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
		
		OnFrameDrawEnd();
	}

	void EditorUIManager::OnFrameDrawBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void EditorUIManager::OnFrameDrawEnd()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* currentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(currentContext);
		}
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
		GetInstance()->sceneViewerPanel->SetTransformOperation(transformOperation);
	}

	void EditorUIManager::SelectGameObjectAt(int x, int y)
	{
		GetInstance()->sceneViewerPanel->SelectGameObjectAt(x, y);
	}

	void EditorUIManager::AddLog(MsgType type, std::string msg)
	{
		GetInstance()->loggerPanel->AddLog(type, msg);
	}

	void EditorUIManager::ClearLogs()
	{
		GetInstance()->loggerPanel->ClearLogs();
	}
}