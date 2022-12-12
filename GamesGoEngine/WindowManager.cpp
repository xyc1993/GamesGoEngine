#include "WindowManager.h"

#include "InputManager.h"
#include "RenderingManager.h"

WindowManager* WindowManager::instance = nullptr;

WindowManager::WindowManager()
{
	glfwInit();

	//sets OpenGL version to 4.4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	//sets profile, core has new features, compat prioritizes compatibility, might not have the newest features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(startWindowWidth, startWindowHeight, "GamesGo Engine", nullptr, nullptr);

	currentScreenWidth = startWindowWidth;
	currentScreenHeight = startWindowHeight;
}

WindowManager* WindowManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new WindowManager();
	}
	return instance;
}

int WindowManager::SetWindow(GLFWwindow* window)
{
	glfwGetFramebufferSize(window, &currentScreenWidth, &currentScreenHeight);

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

	glViewport(0, 0, currentScreenWidth, currentScreenHeight);

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	EnableVSync(vsyncEnabled);

	return EXIT_SUCCESS;
}

bool WindowManager::SetWindow()
{
	const int success = GetInstance()->SetWindow(GetInstance()->window);
	return (success == EXIT_SUCCESS);
}

GLFWwindow* WindowManager::GetWindow()
{
	return GetInstance()->window;
}

void WindowManager::TerminateWindow()
{
	glfwTerminate();
}

GLint WindowManager::GetCurrentWidth()
{
	return GetInstance()->currentScreenWidth;
}

GLint WindowManager::GetCurrentHeight()
{
	return GetInstance()->currentScreenHeight;
}

void WindowManager::EnableVSync(bool enable)
{
	GetInstance()->vsyncEnabled = enable;
	glfwSwapInterval(IsVSyncEnabled() ? 1 : 0);
}

bool WindowManager::IsVSyncEnabled()
{
	return GetInstance()->vsyncEnabled;
}

void WindowManager::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	RenderingManager::ResizeBuffers(width, height);
	GetInstance()->currentScreenWidth = width;
	GetInstance()->currentScreenHeight = height;
}