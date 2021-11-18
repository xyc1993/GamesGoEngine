#pragma once

#include <GLFW/glfw3.h>

/*
 * GLFW specific input manager
 */
class InputManager
{
public:
	static void Init(GLFWwindow* window);
	// should be called at the end of the main loop
	static void LateUpdate();

	// returns true if key is being held during call
	static bool GetKey(int key);
	// returns true if key is being pressed during call
	static bool GetKeyPressed(int key);
	// returns true if key is being released during call
	static bool GetKeyReleased(int key);

	static GLfloat GetMouseXInput();
	static GLfloat GetMouseYInput();
	static GLfloat GetMouseScrollInput();

	// status of cursor, if enabled will be visible, otherwise not
	static bool GetCursorEnabled();
	static void SetCursorEnabled(bool enabled);

private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	static GLFWwindow* window;

	static const int KEYS_NUMBER = 1024;

	static bool keys[KEYS_NUMBER];
	static bool keysPressed[KEYS_NUMBER];
	static bool keysReleased[KEYS_NUMBER];

	static GLfloat mouseXInput;
	static GLfloat mouseYInput;
	static GLfloat mouseScrollInput;
	static GLfloat lastMouseX;
	static GLfloat lastMouseY;
	static bool firstMouse;
	static bool cursorEnabled;
};