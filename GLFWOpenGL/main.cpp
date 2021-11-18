#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "CubePrimitive.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Model.h"
#include "Skybox.h"

#include "LightsManager.h"
#include "Material.h"
#include "MeshRenderer.h"

const GLint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void ProcessMovementInput();
void ProcessDebugInput(GLFWwindow* window);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// not a good way to handle this (especially without encapsulation), but 'projection' and 'view' should be accessible to all renderers so for now it's fine
glm::mat4 view_global;
glm::mat4 projection_global;

void SetCursor(GLFWwindow* window)
{
	if (InputManager::cursorEnabled)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

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

	return glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Engine", nullptr, nullptr);
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
	SetCursor(window);	

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
	GLfloat currentTime = 0.0f;

	glm::vec3 startPointLightPositions[] = {
		glm::vec3(0.07,   0.2f,   1.0f),
		glm::vec3(2.3f,   -3.3f,   -4.0f),
		glm::vec3(-4.0f,  -2.0f,  -11.0f),
		glm::vec3(0.0f,  0.0f,  -3.0f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.07,   0.2f,   2.0f),
		glm::vec3(2.3f,   -3.3f,   -4.0f),
		glm::vec3(-4.0f,  -2.0f,  -11.0f),
		glm::vec3(0.0f,  0.0f,  -3.0f)
	};

	Shader modelShader("res/shaders/modelLoading.vert", "res/shaders/modelLoading.frag");
	Model loadedModel((GLchar*)"res/nanosuit/nanosuit.obj");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //use this for wireframe

	glm::vec3 litBoxesPositions[] = {
		glm::vec3(3.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	glm::vec3 litBoxesRotations[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(30.0f, 15.0f, 0.0f),
		glm::vec3(60.0f, 30.0f, 0.0f),
		glm::vec3(90.0f, 45.0f, 0.0f),
		glm::vec3(120.0f, 60.0f, 0.0f),
		glm::vec3(150.0f, 75.0f, 0.0f),
		glm::vec3(180.0f, 90.0f, 0.0f),
		glm::vec3(210.0f, 105.0f, 0.0f),
		glm::vec3(240.0f, 120.0f, 0.0f),
		glm::vec3(270.0f, 135.0f, 0.0f)
	};

	// SKYBOX
	Skybox skybox = Skybox();

	projection_global = glm::mat4(1.0f);
	projection_global = glm::perspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// LIGHTS BEGIN
	LightsManager::AddDirectionalLight(glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f), glm::vec3(-0.2f, -1.0f, -0.3f));
	for (int i = 0; i < LightsManager::MAX_NUMBER_OF_POINT_LIGHTS; i++)
	{
		LightsManager::AddPointLight(glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), pointLightPositions[i], 1.0f, 0.09f, 0.032f);
	}
	LightsManager::AddSpotLight(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), camera.GetPosition(), camera.GetFront(),
								1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));
	// LIGHTS END

	// NEW CODE DATA STRUCTURE REWORK (remember to add delete after while()!)
	CubePrimitive* cubeMesh = new CubePrimitive();

	Material* lampMaterial = new Material("res/shaders/lamp.vert", "res/shaders/lamp.frag");
	
	const int LAMPS_NUMBER = 4;
	GameObject lampObjects[] = {
		GameObject(),
		GameObject(),
		GameObject(),
		GameObject()
	};

	MeshRenderer* lampMeshRenderers[] = {
		new MeshRenderer(),
		new MeshRenderer(),
		new MeshRenderer(),
		new MeshRenderer()
	};

	for (int i = 0; i < LAMPS_NUMBER; i++)
	{
		lampMeshRenderers[i]->SetMaterial(lampMaterial);
		lampMeshRenderers[i]->SetMesh(cubeMesh);
		lampObjects[i].AddComponent(lampMeshRenderers[i]);
		lampObjects[i].GetTransform()->SetPosition(pointLightPositions[i]);
		lampObjects[i].GetTransform()->SetScale(glm::vec3(0.2f));
	}

	Material* cubeLitMaterial = new Material("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	cubeLitMaterial->AddTexture((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"material.diffuse");
	cubeLitMaterial->AddTexture((GLchar*)"res/box/container2_specular.png", (GLchar*)"material.specular");
	cubeLitMaterial->AddFloat(32.0f, (GLchar*)"material.shininess");
	cubeLitMaterial->SetLightModel(LightModelType::LitForward);

	const int LIT_BOXES_NUMBER = 10;
	GameObject litBoxesObjects[] = {
		GameObject(), GameObject(), GameObject(),
		GameObject(), GameObject(), GameObject(),
		GameObject(), GameObject(), GameObject(),
		GameObject()
	};

	MeshRenderer* litBoxesMeshRenderers[] = {
		new MeshRenderer(), new MeshRenderer(), new MeshRenderer(),
		new MeshRenderer(), new MeshRenderer(), new MeshRenderer(),
		new MeshRenderer(), new MeshRenderer(), new MeshRenderer(),
		new MeshRenderer()
	};

	for (int i = 0; i < LIT_BOXES_NUMBER; i++)
	{
		litBoxesMeshRenderers[i]->SetMaterial(cubeLitMaterial);
		litBoxesMeshRenderers[i]->SetMesh(cubeMesh);
		litBoxesObjects[i].AddComponent(litBoxesMeshRenderers[i]);
		litBoxesObjects[i].GetTransform()->SetPosition(litBoxesPositions[i]);
		litBoxesObjects[i].GetTransform()->SetRotation(litBoxesRotations[i]);
	}
	
	// NEW CODE END
	
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = timeMultiplier * (currentFrame - lastFrame);
		lastFrame = currentFrame;
		currentTime += deltaTime;

		//just for some cool dynamic light effect
		for (int i = 0; i < 4; i++)
		{
			GLfloat index = (GLfloat)i;
			pointLightPositions[i].y = startPointLightPositions[i].y + sin(0.4f * (index + 1.0f) * currentTime);
		}

		glfwPollEvents();
		ProcessMovementInput();
		ProcessDebugInput(window);

		glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		modelShader.Use();
		view_global = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_global));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view_global));

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		loadedModel.Draw(modelShader);

		//SKYBOX
		skybox.Draw(glm::mat4(glm::mat3(camera.GetViewMatrix())), projection_global);

		//LIGHTS BEGIN
		for (int i = 0; i < LightsManager::MAX_NUMBER_OF_POINT_LIGHTS; i++)
		{
			LightsManager::GetPointLight(i)->SetPosition(pointLightPositions[i]);
		}
		LightsManager::GetSpotLight(0)->SetPosition(camera.GetPosition());
		LightsManager::GetSpotLight(0)->SetDirection(camera.GetFront());
		//LIGHTS END
		
		for (int i = 0; i < LIT_BOXES_NUMBER; i++)
		{
			litBoxesObjects[i].Update();
		}
				
		for (int i = 0; i < LAMPS_NUMBER; i++)
		{
			lampObjects[i].GetTransform()->SetPosition(pointLightPositions[i]);
			lampObjects[i].Update();
		}
		
		ImGui::Begin("Test ImGUI window");
		ImGui::Text("ImGUI Text");
		ImGui::SliderFloat("Time Scale", &timeMultiplier, 0.0f, 5.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		//reset pressed & released keys statuses
		InputManager::LateUpdate();
	}

	// NEW CODE CLEANUP
	/*
	delete cubeMesh;
	delete lampMaterial;
	delete lampMeshRenderer;
	*/

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

void ProcessMovementInput()
{	
	if (InputManager::GetKey(GLFW_KEY_W) || InputManager::GetKey(GLFW_KEY_UP))
	{
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}

	if (InputManager::GetKey(GLFW_KEY_S) || InputManager::GetKey(GLFW_KEY_DOWN))	
	{
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	
	if (InputManager::GetKey(GLFW_KEY_A) || InputManager::GetKey(GLFW_KEY_LEFT))
	{
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	
	if (InputManager::GetKey(GLFW_KEY_D) || InputManager::GetKey(GLFW_KEY_RIGHT))
	{
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
	
	if (InputManager::GetKey(GLFW_KEY_E))
	{
		camera.ProcessKeyboard(Camera_Movement::UPWARD, deltaTime);
	}
	
	if (InputManager::GetKey(GLFW_KEY_Q))
	{
		camera.ProcessKeyboard(Camera_Movement::DOWNWARD, deltaTime);
	}

	camera.ProcessMouseMovement(InputManager::GetMouseXInput(), InputManager::GetMouseYInput());
}

void ProcessDebugInput(GLFWwindow* window)
{
	if (InputManager::GetKeyReleased(GLFW_KEY_H))
	{
		InputManager::cursorEnabled = !InputManager::cursorEnabled;
		SetCursor(window);
	}
}