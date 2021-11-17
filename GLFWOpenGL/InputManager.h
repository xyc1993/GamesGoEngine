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

private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static const int KEYS_NUMBER = 1024;

	static bool keys[KEYS_NUMBER];
	static bool keysPressed[KEYS_NUMBER];
	static bool keysReleased[KEYS_NUMBER];
};