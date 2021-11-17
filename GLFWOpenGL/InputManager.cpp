#include "InputManager.h"

bool InputManager::keys[KEYS_NUMBER];
bool InputManager::keysPressed[KEYS_NUMBER];
bool InputManager::keysReleased[KEYS_NUMBER];

void InputManager::Init(GLFWwindow* window)
{
	for (int i = 0; i < KEYS_NUMBER; i++)
	{
		keys[i] = false;
		keysPressed[i] = false;
		keysReleased[i] = false;
	}

	glfwSetKeyCallback(window, KeyCallback);
}

void InputManager::LateUpdate()
{
	for (int i = 0; i < KEYS_NUMBER; i++)
	{
		// reset the state of the 'pressed' & 'released' keys since their status lasts only 1 frame
		keysPressed[i] = false;
		keysReleased[i] = false;
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

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// this shouldn't be here but for now let's do this for quick implementation
	if (key == GLFW_KEY_ESCAPE && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

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