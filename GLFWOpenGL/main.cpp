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
#include "MeshPrimitiveCube.h"
#include "EditorMovement.h"
#include "GameObject.h"
#include "InputEditorShortcuts.h"
#include "InputManager.h"
#include "Skybox.h"

#include "LightsManager.h"
#include "Material.h"
#include "MeshImported.h"
#include "MeshPrimitiveQuad.h"
#include "MeshRenderer.h"

const GLint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

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
		glm::vec3(120.0f,60.0f, 0.0f),
		glm::vec3(150.0f,75.0f, 0.0f),
		glm::vec3(180.0f,90.0f, 0.0f),
		glm::vec3(210.0f,105.0f, 0.0f),
		glm::vec3(240.0f,120.0f, 0.0f),
		glm::vec3(270.0f,135.0f, 0.0f)
	};

	// SKYBOX
	Skybox skybox = Skybox();

	projection_global = glm::mat4(1.0f);
	projection_global = glm::perspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	
	// NEW CODE DATA STRUCTURE REWORK (remember to add delete after while()!)
	MeshPrimitiveCube* cubeMesh = new MeshPrimitiveCube();
	//MeshPrimitiveQuad* quadMesh = new MeshPrimitiveQuad();

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
		lampMeshRenderers[i]->SetMesh(cubeMesh);
		lampMeshRenderers[i]->SetMaterial(lampMaterial);
		lampObjects[i].AddComponent(lampMeshRenderers[i]);
		lampObjects[i].GetTransform()->SetPosition(pointLightPositions[i]);
		lampObjects[i].GetTransform()->SetScale(glm::vec3(0.2f));

		PointLight* pointLight = new PointLight(glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
		lampObjects[i].AddComponent(pointLight);
	}

	Material* cubeLitMaterial = new Material("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	cubeLitMaterial->SetTexture((GLchar*)"material.diffuse", 0, (GLchar*)"res/box/container2_diffuse.png");
	cubeLitMaterial->SetTexture((GLchar*)"material.specular", 1, (GLchar*)"res/box/container2_specular.png");
	cubeLitMaterial->SetFloat((GLchar*)"material.shininess", 32.0f);
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
		litBoxesMeshRenderers[i]->SetMesh(cubeMesh);
		litBoxesMeshRenderers[i]->SetMaterial(cubeLitMaterial);
		litBoxesObjects[i].AddComponent(litBoxesMeshRenderers[i]);
		litBoxesObjects[i].GetTransform()->SetPosition(litBoxesPositions[i]);
		litBoxesObjects[i].GetTransform()->SetRotationEulerDegrees(litBoxesRotations[i]);
	}

	GameObject nanoSuitObject = GameObject();
	nanoSuitObject.GetTransform()->SetPosition(glm::vec3(0.0f, -1.75f, 0.0f));
	nanoSuitObject.GetTransform()->SetScale(glm::vec3(0.2f));
	MeshRenderer* nanoSuitMeshRenderer = new MeshRenderer();
	MeshImported* nanoSuitMesh = new MeshImported((GLchar*)"res/nanosuit/nanosuit.obj");
	nanoSuitMeshRenderer->SetMesh(nanoSuitMesh);

	const int NANOSUIT_MATERIALS_NUMBER = 7;
	Material* nanoSuitMaterials[] = {
		new Material(), new Material(), new Material(),
		new Material(), new Material(), new Material(),
		new Material()
	};

	std::vector<GLchar*> nanoSuitTexturePaths;
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/glass_dif.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/leg_dif.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/hand_dif.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/glass_dif.png"); // this one is small 'lights' on the helmet under visor, no specific texture was given for those
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/arm_dif.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/helmet_diff.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/body_dif.png");

	for (int i = 0; i < NANOSUIT_MATERIALS_NUMBER; i++)
	{
		nanoSuitMaterials[i] = new Material("res/shaders/lighting.vert", "res/shaders/lighting.frag");
		nanoSuitMaterials[i]->SetTexture((GLchar*)"material.diffuse", 0, nanoSuitTexturePaths[i]);
		nanoSuitMaterials[i]->SetFloat((GLchar*)"material.shininess", 32.0f);
		nanoSuitMaterials[i]->SetLightModel(LightModelType::LitForward);
		nanoSuitMeshRenderer->SetMaterial(nanoSuitMaterials[i], i);
	}

	nanoSuitObject.AddComponent(nanoSuitMeshRenderer);

	GameObject editorSpectatorObject = GameObject();
	editorSpectatorObject.GetTransform()->SetRotationEulerDegrees(glm::vec3(0.0f, 180.0f, 0.0f));
	editorSpectatorObject.GetTransform()->SetPosition(glm::vec3(0.0f, 0.0, 3.0f));

	EditorMovement* editorMovementComponent = new EditorMovement();
	Camera* cameraComponent = new Camera();
	SpotLight* spotLight = new SpotLight(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), 
		1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));

	editorSpectatorObject.AddComponent(editorMovementComponent);
	editorSpectatorObject.AddComponent(cameraComponent);
	editorSpectatorObject.AddComponent(spotLight);

	GameObject directionalLightObject = GameObject();
	directionalLightObject.GetTransform()->SetRotationEulerDegrees(glm::vec3(90.0f, -45.0f, 0.0f));
	DirectionalLight* directionalLight = new DirectionalLight(glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	directionalLightObject.AddComponent(directionalLight);
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
		InputEditorShortcuts::ProcessShortcuts(window);

		glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		editorSpectatorObject.Update();
		directionalLightObject.Update();
		
		//SKYBOX
		skybox.Draw(glm::mat4(glm::mat3(view_global)), projection_global);

		nanoSuitObject.Update();
				
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