#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLEW_STATIC
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "CamerasManager.h"
#include "InputEditorShortcuts.h"
#include "InputManager.h"
#include "PropertiesUI.h"
#include "SceneExample_DepthTest.h"
#include "SceneExample_LitForward.h"
#include "Time.h"
#include "WorldOutlinerUI.h"

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
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	bool wireframeOnly = false;
	
	SceneExample_LitForward* activeScene = new SceneExample_LitForward();
	//SceneExample_DepthTest* activeScene = new SceneExample_DepthTest();
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GameObject* selectedSceneObject = nullptr;
	
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();

		CamerasManager::SetCurrentViewAspectRatio((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
		
		glfwPollEvents();
		InputEditorShortcuts::ProcessShortcuts(window);

		glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		activeScene->Update();

		GLfloat fps = 1.0f / Time::GetUnscaledDeltaTime();
		std::string fpsText = "FPS = ";
		fpsText.append(std::to_string(fps));

		ImGui::Begin("Basic ImGUI window");
		ImGui::Text(fpsText.c_str());
		float timeScale = Time::GetTimeScale();
		if (ImGui::SliderFloat("Time Scale", &timeScale, 0.0f, 5.0f))
		{
			Time::SetTimeScale(timeScale);
		}
		if (ImGui::Checkbox("Wireframe only", &wireframeOnly))
		{
			glPolygonMode(GL_FRONT_AND_BACK, wireframeOnly ? GL_LINE : GL_FILL);
		}
		ImGui::End();

		selectedSceneObject = WorldOutlinerUI::Draw(activeScene->GetSceneUnsafe());
		PropertiesUI::Draw(selectedSceneObject);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		//reset pressed & released keys statuses
		InputManager::LateUpdate();
	}

	delete activeScene;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int main()
{
	GLFWwindow* window = InitWindow();
	
	int success = SetWindow(window);
	if (success != 0) return EXIT_FAILURE;

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	MainLoop(window);

	glfwTerminate();
	
	return EXIT_SUCCESS;
}