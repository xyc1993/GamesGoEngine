#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLEW_STATIC
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "InputEditorShortcuts.h"
#include "InputManager.h"

#include "SceneExample_LitForward.h"
#include "WorldOutlinerUI.h"

const GLint WIDTH = 1200, HEIGHT = 675;
int SCREEN_WIDTH, SCREEN_HEIGHT;

GLfloat currentTime = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// not a good way to handle this (especially without encapsulation), but 'projection' and 'view' should be accessible to all renderers so for now it's fine
glm::mat4 view_global;
glm::mat4 projection_global;

GLFWwindow* InitWindow()
{
	glfwInit();

	//sets OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//sets profile, core has new features, compat prioritizes compatibility, might not have the newest features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //window won't be resizable

	return glfwCreateWindow(WIDTH, HEIGHT, "GamesGo Engine", nullptr, nullptr);
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

	GLfloat timeMultiplier = 1.0f;
	currentTime = 0.0f;

	bool wireframeOnly = false;
	bool lastWireframeOnly = false;
	
	SceneExample_LitForward* activeScene = new SceneExample_LitForward();
	
	projection_global = glm::mat4(1.0f);
	projection_global = glm::perspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int selectedSceneObjectIndex = -1;
	
	while (!glfwWindowShouldClose(window))
	{
		if (wireframeOnly != lastWireframeOnly)
		{
			lastWireframeOnly = wireframeOnly;
			if (wireframeOnly)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}

		GLfloat currentFrame = glfwGetTime();
		deltaTime = timeMultiplier * (currentFrame - lastFrame);
		lastFrame = currentFrame;
		currentTime += deltaTime;
		
		glfwPollEvents();
		InputEditorShortcuts::ProcessShortcuts(window, activeScene->GetSceneUnsafe(), selectedSceneObjectIndex);

		glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		activeScene->Update();

		GLfloat fps = 1.0f / deltaTime;
		std::string fpsText = "FPS = ";
		fpsText.append(std::to_string(fps));

		ImGui::Begin("Basic ImGUI window");
		ImGui::Text(fpsText.c_str());
		ImGui::SliderFloat("Time Scale", &timeMultiplier, 0.0f, 5.0f);
		ImGui::Checkbox("Wireframe only", &wireframeOnly);
		ImGui::End();

		selectedSceneObjectIndex = WorldOutlinerUI::Draw(activeScene->GetScene());

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	MainLoop(window);

	glfwTerminate();
	
	return EXIT_SUCCESS;
}