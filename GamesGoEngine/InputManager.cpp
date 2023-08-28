#include "InputManager.h"

namespace GamesGoEngine
{
	InputManager* InputManager::instance = new InputManager();

	InputManager::InputManager()
	{
		for (int i = 0; i < KEYS_NUMBER; i++)
		{
			keys[i] = false;
			keysPressed[i] = false;
			keysReleased[i] = false;
		}

		firstMouse = true;
		mouseXInput = 0.0f;
		mouseYInput = 0.0f;
	}

	InputManager* InputManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new InputManager();
		}
		return instance;
	}

	void InputManager::Init(GLFWwindow* window)
	{
		GetInstance()->window = window;

		int width = 0, height = 0;
		glfwGetWindowSize(window, &width, &height);
		GetInstance()->lastMouseX = (float)width / 2.0f;
		GetInstance()->lastMouseY = (float)height / 2.0f;

		SetCursorEnabled(true);

		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetCursorPosCallback(window, MousePosCallback);
		glfwSetScrollCallback(window, ScrollCallback);
	}

	void InputManager::LateUpdate()
	{
		for (int i = 0; i < KEYS_NUMBER; i++)
		{
			// reset the state of the 'pressed' & 'released' keys since their status lasts only 1 frame
			GetInstance()->keysPressed[i] = false;
			GetInstance()->keysReleased[i] = false;
		}

		for (int i = 0; i < MOUSE_BUTTONS_NUMBER; i++)
		{
			// reset the state of the 'pressed' & 'released' buttons since their status lasts only 1 frame
			GetInstance()->mouseButtonsPressed[i] = false;
			GetInstance()->mouseButtonsReleased[i] = false;
		}

		// after mouse input was used, reset it to avoid situation where there's no mouse callback but there's mouse input data
		GetInstance()->mouseXInput = 0.0f;
		GetInstance()->mouseYInput = 0.0f;
	}

	bool InputManager::GetKey(int key)
	{
		if (key >= 0 && key < KEYS_NUMBER)
		{
			return GetInstance()->keys[key];
		}
		return false;
	}

	bool InputManager::GetKeyPressed(int key)
	{
		if (key >= 0 && key < KEYS_NUMBER)
		{
			return GetInstance()->keysPressed[key];
		}
		return false;
	}

	bool InputManager::GetKeyReleased(int key)
	{
		if (key >= 0 && key < KEYS_NUMBER)
		{
			return GetInstance()->keysReleased[key];
		}
		return false;
	}

	bool InputManager::GetMouseButton(int button)
	{
		if (button >= 0 && button < MOUSE_BUTTONS_NUMBER)
		{
			return GetInstance()->mouseButtons[button];
		}
		return false;
	}

	bool InputManager::GetMouseButtonPressed(int button)
	{
		if (button >= 0 && button < MOUSE_BUTTONS_NUMBER)
		{
			return GetInstance()->mouseButtonsPressed[button];
		}
		return false;
	}

	bool InputManager::GetMouseButtonReleased(int button)
	{
		if (button >= 0 && button < MOUSE_BUTTONS_NUMBER)
		{
			return GetInstance()->mouseButtonsReleased[button];
		}
		return false;
	}

	GLfloat InputManager::GetMouseXInput()
	{
		return GetInstance()->mouseXInput;
	}

	GLfloat InputManager::GetMouseYInput()
	{
		return GetInstance()->mouseYInput;
	}

	GLfloat InputManager::GetMouseScrollInput()
	{
		return GetInstance()->mouseScrollInput;
	}

	bool InputManager::GetCursorEnabled()
	{
		return GetInstance()->cursorEnabled;
	}

	void InputManager::SetCursorEnabled(bool enabled)
	{
		GetInstance()->cursorEnabled = enabled;

		if (GetInstance()->window != nullptr)
		{
			if (GetInstance()->cursorEnabled)
			{
				glfwSetInputMode(GetInstance()->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(GetInstance()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}

	void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key >= 0 && key < KEYS_NUMBER)
		{
			if (GLFW_PRESS == action)
			{
				// if the key wasn't held in the last frame then it is pressed now
				GetInstance()->keysPressed[key] = !GetInstance()->keys[key];

				GetInstance()->keysReleased[key] = false;
				GetInstance()->keys[key] = true;
			}
			else if (GLFW_RELEASE == action)
			{
				GetInstance()->keys[key] = false;
				GetInstance()->keysPressed[key] = false;
				GetInstance()->keysReleased[key] = true;
			}
		}
	}

	void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
	{
		if (button >= 0 && button < MOUSE_BUTTONS_NUMBER)
		{
			if (GLFW_PRESS == action)
			{
				// if the key wasn't held in the last frame then it is pressed now
				GetInstance()->mouseButtonsPressed[button] = !GetInstance()->mouseButtons[button];

				GetInstance()->mouseButtonsReleased[button] = false;
				GetInstance()->mouseButtons[button] = true;
			}
			else if (GLFW_RELEASE == action)
			{
				GetInstance()->mouseButtons[button] = false;
				GetInstance()->mouseButtonsPressed[button] = false;
				GetInstance()->mouseButtonsReleased[button] = true;
			}
		}
	}

	void InputManager::MousePosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (GetInstance()->firstMouse)
		{
			GetInstance()->lastMouseX = xPos;
			GetInstance()->lastMouseY = yPos;
			GetInstance()->firstMouse = false;
		}

		if (!GetInstance()->cursorEnabled)
		{
			GetInstance()->mouseXInput = xPos - GetInstance()->lastMouseX;
			GetInstance()->mouseYInput = GetInstance()->lastMouseY - yPos; //reversed because goes the other way around
		}

		GetInstance()->lastMouseX = xPos;
		GetInstance()->lastMouseY = yPos;
	}

	void InputManager::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		GetInstance()->mouseScrollInput = yOffset;
	}
}