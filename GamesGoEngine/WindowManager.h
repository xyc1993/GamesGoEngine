#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowManager
{
private:
	WindowManager();

	static WindowManager* instance;
	static WindowManager* GetInstance();
	
	int SetWindow(GLFWwindow* window);

public:
	static bool SetWindow();
	static GLFWwindow* GetWindow();
	static void TerminateWindow();
	static GLint GetCurrentWidth();
	static GLint GetCurrentHeight();
	static void EnableVSync(bool enable);
	static bool IsVSyncEnabled();

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	GLFWwindow* window;

	const GLint startWindowWidth = 1200;
	const GLint startWindowHeight = 675;

	int currentScreenWidth;
	int currentScreenHeight;

	bool vsyncEnabled;
};