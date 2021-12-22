#pragma once

#include <GLFW/glfw3.h>

/*
 * GLFW specific input manager
 */
class InputManager
{
private:
	InputManager();
	~InputManager() = default;

	static InputManager* instance;
	static InputManager* GetInstance();

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

	static bool GetMouseButton(int button);
	static bool GetMouseButtonPressed(int button);
	static bool GetMouseButtonReleased(int button);

	static GLfloat GetMouseXInput();
	static GLfloat GetMouseYInput();
	static GLfloat GetMouseScrollInput();

	// status of cursor, if enabled will be visible, otherwise not
	static bool GetCursorEnabled();
	static void SetCursorEnabled(bool enabled);

private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mode);
	static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);	
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	GLFWwindow* window;

	static constexpr int KEYS_NUMBER = 1024;
	static constexpr int MOUSE_BUTTONS_NUMBER = 8;

	bool keys[KEYS_NUMBER];
	bool keysPressed[KEYS_NUMBER];
	bool keysReleased[KEYS_NUMBER];

	bool mouseButtons[MOUSE_BUTTONS_NUMBER];
	bool mouseButtonsPressed[MOUSE_BUTTONS_NUMBER];
	bool mouseButtonsReleased[MOUSE_BUTTONS_NUMBER];

	GLfloat mouseXInput;
	GLfloat mouseYInput;
	GLfloat mouseScrollInput;
	GLfloat lastMouseX;
	GLfloat lastMouseY;
	bool firstMouse;
	bool cursorEnabled;
};