#include "InputManager.h"

GLFWwindow* InputManager::window;

bool InputManager::keys[KEYS_NUMBER];
bool InputManager::keysPressed[KEYS_NUMBER];
bool InputManager::keysReleased[KEYS_NUMBER];

GLfloat InputManager::mouseXInput = 0.0f;
GLfloat InputManager::mouseYInput = 0.0f;
GLfloat InputManager::lastMouseX;
GLfloat InputManager::lastMouseY;
GLfloat InputManager::mouseScrollInput;
bool InputManager::firstMouse = true;
bool InputManager::cursorEnabled = false;

void InputManager::Init(GLFWwindow* window)
{
	InputManager::window = window;

	for (int i = 0; i < KEYS_NUMBER; i++)
	{
		keys[i] = false;
		keysPressed[i] = false;
		keysReleased[i] = false;
	}

	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	lastMouseX = (float)width / 2.0f;
	lastMouseY = (float)height / 2.0f;

	SetCursorEnabled(cursorEnabled);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
}

void InputManager::LateUpdate()
{
	for (int i = 0; i < KEYS_NUMBER; i++)
	{
		// reset the state of the 'pressed' & 'released' keys since their status lasts only 1 frame
		keysPressed[i] = false;
		keysReleased[i] = false;

		// after mouse input was used, reset it to avoid situation where there's no mouse callback but there's mouse input data
		mouseXInput = 0.0f;
		mouseYInput = 0.0f;
	}
}

bool InputManager::GetKey(int key)
{
	if (key >= 0 && key < 1024)
	{
		return keys[key];
	}
	return false;
}

bool InputManager::GetKeyPressed(int key)
{
	if (key >= 0 && key < 1024)
	{
		return keysPressed[key];
	}
	return false;
}

bool InputManager::GetKeyReleased(int key)
{
	if (key >= 0 && key < 1024)
	{
		return keysReleased[key];
	}
	return false;
}

GLfloat InputManager::GetMouseXInput()
{
	return mouseXInput;
}

GLfloat InputManager::GetMouseYInput()
{
	return mouseYInput;
}

GLfloat InputManager::GetMouseScrollInput()
{
	return mouseScrollInput;
}

bool InputManager::GetCursorEnabled()
{
	return cursorEnabled;
}

void InputManager::SetCursorEnabled(bool enabled)
{
	cursorEnabled = enabled;

	if (window != nullptr)
	{
		if (cursorEnabled)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (GLFW_PRESS == action)
		{
			// if the key wasn't held in the last frame then it is pressed now
			keysPressed[key] = !keys[key];

			keysReleased[key] = false;
			keys[key] = true;
		}
		else if (GLFW_RELEASE == action)
		{
			keys[key] = false;
			keysPressed[key] = false;
			keysReleased[key] = true;
		}
	}
}

void InputManager::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastMouseX = xPos;
		lastMouseY = yPos;
		firstMouse = false;
	}

	if (!cursorEnabled)
	{
		mouseXInput = xPos - lastMouseX;
		mouseYInput = lastMouseY - yPos; //reversed because goes the other way around
	}
	
	lastMouseX = xPos;
	lastMouseY = yPos;
}

void InputManager::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	mouseScrollInput = yOffset;
}