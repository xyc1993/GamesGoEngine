#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "CamerasManager.h"
#include "EditorUIManager.h"
#include "InputEditorShortcuts.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "Time.h"
#include "WindowManager.h"

#include "SceneExamples.h"
#include "SceneManager.h"

using namespace GamesGoEngine;

void MainLoop(GLFWwindow* window)
{
	EditorUIManager::Init(window);
	RenderingManager::Init();
	
	//SceneExample_LitForward* activeScene = new SceneExample_LitForward(false);
	//SceneExample_LitForward* activeScene = new SceneExample_LitForward(true);
	//SceneExample_DepthTest* activeScene = new SceneExample_DepthTest();
	//SceneExample_Transparency* activeScene = new SceneExample_Transparency();
	//SceneExample_EnvironmentMapping* activeScene = new SceneExample_EnvironmentMapping();
	//SceneExample_GeometryShader* activeScene = new SceneExample_GeometryShader();
	//SceneExample_Instancing* activeScene = new SceneExample_Instancing();
	//SceneExample_DirectionalShadows* activeScene = new SceneExample_DirectionalShadows();
	//SceneExample_OmnidirectionalShadows* activeScene = new SceneExample_OmnidirectionalShadows();
	SceneExample_DeferredRendering* activeScene = new SceneExample_DeferredRendering();
	//SceneExample_MultipleLightsShadows* activeScene = new SceneExample_MultipleLightsShadows();
	//SceneExample_HangarPBR* activeScene = new SceneExample_HangarPBR();

	SceneManager::SetActiveScene(activeScene->GetSceneUnsafe());

	while (!glfwWindowShouldClose(window))
	{
		Time::Update();

		CamerasManager::SetCurrentViewAspectRatio(RenderingManager::GetBufferAspectRatio());
		
		glfwPollEvents();
		InputEditorShortcuts::ProcessShortcuts(window, activeScene->GetSceneUnsafe());
		
		activeScene->Update();
		RenderingManager::Update();
		EditorUIManager::Draw();
		
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