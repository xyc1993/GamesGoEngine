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
#include "SceneExample_LitForward.h"
#include "SceneExample_Transparency.h"
#include "Time.h"

const GLint WIDTH = 1200, HEIGHT = 675;
int SCREEN_WIDTH, SCREEN_HEIGHT;

GLFWwindow* InitWindow()
{
	glfwInit();

	//sets OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//sets profile, core has new features, compat prioritizes compatibility, might not have the newest features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	return glfwCreateWindow(WIDTH, HEIGHT, "GamesGo Engine", nullptr, nullptr);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	RenderingManager::ResizeBuffers(width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

int SetWindow(GLFWwindow* window)
{
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	InputManager::Init(window);

	glewExperimental = GL_TRUE; //GLEW will use modern approach, it's not 'experimental' per se

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	return EXIT_SUCCESS;
}

void MainLoop(GLFWwindow* window)
{
	RenderingManager::Init(SCREEN_WIDTH, SCREEN_HEIGHT);
	EditorUIManager::Init(window);
	
	SceneExample_LitForward* activeScene = new SceneExample_LitForward();
	//SceneExample_DepthTest* activeScene = new SceneExample_DepthTest();
	//SceneExample_Transparency* activeScene = new SceneExample_Transparency();
	
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();

		CamerasManager::SetCurrentViewAspectRatio((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
		
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
	GLFWwindow* window = InitWindow();
	const int success = SetWindow(window);
	if (success != 0) return EXIT_FAILURE;
	
	MainLoop(window);

	glfwTerminate();
	
	return EXIT_SUCCESS;
}