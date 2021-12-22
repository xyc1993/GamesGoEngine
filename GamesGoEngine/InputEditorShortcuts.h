#pragma once

#include <GLFW/glfw3.h>

#include "Scene.h"

class InputEditorShortcuts
{
public:
	static void ProcessShortcuts(GLFWwindow* window, Scene* activeScene, int selectedGameObjectIndex);
};