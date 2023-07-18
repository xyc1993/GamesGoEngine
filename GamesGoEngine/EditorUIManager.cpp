#include "EditorUIManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "LoggerUI.h"
#include "PropertiesUI.h"
#include "WorldOutlinerUI.h"
#include "DebugToolsUI.h"
#include "GraphicsSettingsUI.h"

float EditorUIManager::windowWidth = 0.0f;
float EditorUIManager::windowHeight = 0.0f;

void EditorUIManager::Init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void EditorUIManager::Draw(Scene* activeScene)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGuiIO& io = ImGui::GetIO();
	//io.FontGlobalScale = 3.0f;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(200.0f, 300.0f));

	GameObject* selectedSceneObject = WorldOutlinerUI::Draw(activeScene);

	ImGui::SetNextWindowPos(ImVec2(windowWidth - 300.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(300.0f, 300.0f));

	PropertiesUI::Draw(selectedSceneObject);
	
	ImGui::SetNextWindowPos(ImVec2(0.0f, windowHeight - 300.0f));
	ImGui::SetNextWindowSize(ImVec2(400.0f, 300.0f));

	GraphicsSettingsUI::Draw();

	ImGui::SetNextWindowPos(ImVec2(0.5f * windowWidth - 400.0f, windowHeight - 300.0f));
	ImGui::SetNextWindowSize(ImVec2(800.0f, 300.0f));

	LoggerUI::Draw();

	ImGui::SetNextWindowPos(ImVec2(windowWidth - 400.0f, windowHeight - 300.0f));
	ImGui::SetNextWindowSize(ImVec2(400.0f, 300.0f));

	DebugToolsUI::Draw();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUIManager::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorUIManager::UpdateWindowSize(float width, float height)
{
	windowWidth = width;
	windowHeight = height;
}