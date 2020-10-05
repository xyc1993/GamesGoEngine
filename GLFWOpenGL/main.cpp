#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Cube.h"
#include "Meshes.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "TextureLoader.h"

#include "LightsManager.h"

const GLint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void DoMovement();
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE; //GLEW will use modern approach, it's not 'experimental' per se

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	return EXIT_SUCCESS;
}

void MainLoop(GLFWwindow* window, Shader lightingShader, Shader lampShader)
{
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

	Cube cubes[] = {
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(3.0f, 0.0f, 0.0f), 0.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(2.0f, 5.0f, -15.0f), 20.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(-1.5f, -2.2f, -2.5f), 40.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(-3.8f, -2.0f, -12.3f), 80.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(2.4f, -0.4f, -3.5f), 100.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(-1.7f, 3.0f, -7.5f), 120.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(1.3f, -2.0f, -2.5f), 140.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(1.5f, 2.0f, -2.5f), 160.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(1.5f, 0.2f, -1.5f), 180.0f, 1.0f),
		Cube((GLchar*)"res/box/container2_diffuse.png", (GLchar*)"res/box/container2_specular.png", lightingShader, glm::vec3(-1.3f, 1.0f, -1.5f), 200.0f, 1.0f)
	};

	Cube lampCubes[] = {
		Cube(lampShader, pointLightPositions[0], 0.0f, 0.2f),
		Cube(lampShader, pointLightPositions[1], 0.0f, 0.2f),
		Cube(lampShader, pointLightPositions[2], 0.0f, 0.2f),
		Cube(lampShader, pointLightPositions[3], 0.0f, 0.2f)
	};

	//SKYBOX
	Shader skyboxShader("res/shaders/skybox.vert", "res/shaders/skybox.frag");

	GLuint skyboxVAO, skyyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindVertexArray(0);

	std::vector<const GLchar*> faces;
	faces.push_back("res/images/skybox/right.tga");
	faces.push_back("res/images/skybox/left.tga");
	faces.push_back("res/images/skybox/top.tga");
	faces.push_back("res/images/skybox/bottom.tga");
	faces.push_back("res/images/skybox/back.tga");
	faces.push_back("res/images/skybox/front.tga");	

	GLuint cubemapTexture = TextureLoader::LoadCubemap(faces);
	//SKYBOX END

	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);	

	//LIGHTS BEGIN
	LightsManager lightsManager = LightsManager(lightingShader.Program);
	lightsManager.AddDirectionalLight(glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f), glm::vec3(-0.2f, -1.0f, -0.3f));
	for (int i = 0; i < lightsManager.MAX_NUMBER_OF_POINT_LIGHTS; i++)
	{
		lightsManager.AddPointLight(glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), pointLightPositions[i], 1.0f, 0.09f, 0.032f);
	}
	lightsManager.AddSpotLight(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), camera.GetPosition(), camera.GetFront(),
								1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));
	//LIGHTS END

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//just for some cool dynamic light effect
		for (int i = 0; i < 4; i++)
		{
			GLfloat index = (GLfloat)i;
			pointLightPositions[i].y = startPointLightPositions[i].y + sin(0.4f * (index + 1.0f) * currentFrame);
		}

		glfwPollEvents();
		DoMovement();

		glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelShader.Use();
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		loadedModel.Draw(modelShader);

		//SKYBOX
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
		//SKYBOX END

		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		//set material settings
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//LIGHTS BEGIN
		for (int i = 0; i < lightsManager.MAX_NUMBER_OF_POINT_LIGHTS; i++)
		{
			lightsManager.GetPointLight(i)->SetPosition(pointLightPositions[i]);
		}
		lightsManager.GetSpotLight(0)->SetPosition(camera.GetPosition());
		lightsManager.GetSpotLight(0)->SetDirection(camera.GetFront());
		lightsManager.SetLightsInShader();
		//LIGHTS END

		view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		for (GLuint i = 0; i < 10; i++)
		{
			cubes[i].Draw(lightingShader);
		}

		lampShader.Use();

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		view = camera.GetViewMatrix();
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		for (GLuint i = 0; i < 4; i++)
		{
			lampCubes[i].SetPosition(pointLightPositions[i]);
			lampCubes[i].Draw(lampShader);
		}

		glfwSwapBuffers(window);
	}
}

int main()
{
	GLFWwindow* window = InitWindow();
	
	int success = SetWindow(window);
	if (success != 0) return EXIT_FAILURE;

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Shader lightingShader("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	Shader lampShader("res/shaders/lamp.vert", "res/shaders/lamp.frag");
	
	MainLoop(window, lightingShader, lampShader);

	glfwTerminate();
	
	return EXIT_SUCCESS;
}

void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}

	if (keys[GLFW_KEY_E])
	{
		camera.ProcessKeyboard(Camera_Movement::UPWARD, deltaTime);
	}

	if (keys[GLFW_KEY_Q])
	{
		camera.ProcessKeyboard(Camera_Movement::DOWNWARD, deltaTime);
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (GLFW_PRESS == action)
		{
			keys[key] = true;
		}
		else if (GLFW_RELEASE == action)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos; //reversed because goes the other way around

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset * deltaTime);
}