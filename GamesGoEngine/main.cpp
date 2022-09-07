#include <iostream>

#define GLEW_STATIC
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "CamerasManager.h"
#include "EditorUIManager.h"
#include "InputEditorShortcuts.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "SceneExample_DepthTest.h"
#include "SceneExample_EnvironmentMapping.h"
#include "SceneExample_GeometryShader.h"
#include "SceneExample_Instancing.h"
#include "SceneExample_LitForward.h"
#include "SceneExample_Transparency.h"
#include "Time.h"
#include "WindowManager.h"

void MainLoop(GLFWwindow* window)
{
	RenderingManager::Init(WindowManager::GetCurrentWidth(), WindowManager::GetCurrentHeight());
	EditorUIManager::Init(window);
	
	//SceneExample_LitForward* activeScene = new SceneExample_LitForward();
	//SceneExample_DepthTest* activeScene = new SceneExample_DepthTest();
	//SceneExample_Transparency* activeScene = new SceneExample_Transparency();
	SceneExample_EnvironmentMapping* activeScene = new SceneExample_EnvironmentMapping();
	//SceneExample_GeometryShader* activeScene = new SceneExample_GeometryShader();
	//SceneExample_Instancing* activeScene = new SceneExample_Instancing();
	
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();

		CamerasManager::SetCurrentViewAspectRatio(static_cast<float>(WindowManager::GetCurrentWidth()) / static_cast<float>(WindowManager::GetCurrentHeight()));
		
		glfwPollEvents();
		InputEditorShortcuts::ProcessShortcuts(window);
		
		activeScene->Update();
		RenderingManager::Update();
		EditorUIManager::Draw(activeScene->GetSceneUnsafe());
		
		glfwSwapBuffers(window);

		//reset pressed & released keys statuses
		InputManager::LateUpdate();
	}

	delete activeScene;
	EditorUIManager::Shutdown();
}

int main()
{
	if (WindowManager::SetWindow() == false) return EXIT_FAILURE;	
	MainLoop(WindowManager::GetWindow());
	WindowManager::TerminateWindow();
	return EXIT_SUCCESS;
}