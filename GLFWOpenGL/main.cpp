#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Meshes.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "TextureLoader.h"

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

glm::vec3 startLightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightPos(startLightPos.x, startLightPos.y, startLightPos.z);

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

void LoadTexture(GLuint& map, const char* filename)
{
	glGenTextures(1, &map);
	int textureWidth, textureHeight;
	unsigned char* image;
		
	//diffuse map
	image = SOIL_load_image(filename, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
}

void GenerateBuffersAndVertexArray(GLuint& VBO, GLuint& VAO, GLuint& lightVAO, GLuint& texture)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //unbind VAO

	//light
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); //unbind VAO
}

void ApplyTextures(GLuint& diffuseMap, GLuint& specularMap, Shader lightingShader)
{
	LoadTexture(diffuseMap, "res/images/container2.png");
	LoadTexture(specularMap, "res/images/container2_specular.png");
	glBindTexture(GL_TEXTURE_2D, 0); //unbind textures

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
}

void MainLoop(GLFWwindow* window, Shader lightingShader, Shader lampShader, GLuint& VAO, GLuint& lightVAO, GLuint& texture)
{
	Shader modelShader("res/shaders/modelLoading.vert", "res/shaders/modelLoading.frag");
	Model loadedModel((GLchar*)"res/models/nanosuit.obj");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //use this for wireframe

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

	vector<const GLchar*> faces;
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

	GLuint diffuseMap, specularMap;
	ApplyTextures(diffuseMap, specularMap, lightingShader);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	glm::vec3 startPointLightPositions[] = {
		glm::vec3(0.07,   0.2f,   2.0f),
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

		//lightingShader.Use();
		//GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		//glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		////set material settings
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//// Directional light
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		//// Point light 1
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		//// Point light 2
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		//// Point light 3
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		//// Point light 4
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		//// SpotLight
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		//glm::mat4 view;
		//view = camera.GetViewMatrix();

		//GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		//GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		//GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, diffuseMap);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMap);

		//glBindVertexArray(VAO);
		//for (GLuint i = 0; i < 10; i++)
		//{
		//	glm::mat4 model(1.0f);
		//	model = glm::translate(model, cubePositions[i]);

		//	GLfloat angle = 20.0f * i;
		//	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);			
		//}
		//glBindVertexArray(0);

		//lampShader.Use();

		//modelLoc = glGetUniformLocation(lampShader.Program, "model");
		//viewLoc = glGetUniformLocation(lampShader.Program, "view");
		//projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//for (GLuint i = 0; i < 4; i++)
		//{
		//	glm::mat4 model(1.0f);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glBindVertexArray(lightVAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
}

void CleanUp(GLuint& VBO, GLuint &VAO, GLuint& lightVAO)
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);	
	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();
}

int main()
{
	GLFWwindow* window = InitWindow();
	
	int success = SetWindow(window);
	if (success != 0) return EXIT_FAILURE;
	
	GLuint VBO, boxVAO, lightVAO, texture;

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Shader lightingShader("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	Shader lampShader("res/shaders/lamp.vert", "res/shaders/lamp.frag");

	GenerateBuffersAndVertexArray(VBO, boxVAO, lightVAO, texture);
	
	MainLoop(window, lightingShader, lampShader, boxVAO, lightVAO, texture);

	CleanUp(VBO, boxVAO, lightVAO);
	
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