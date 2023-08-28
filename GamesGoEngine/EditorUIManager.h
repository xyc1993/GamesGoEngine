#pragma once

#include <GLFW/glfw3.h>

namespace GamesGoEngine
{
	class Scene;

	class EditorUIManager
	{
	public:
		static void Init(GLFWwindow* window);
		static void Draw(Scene* activeScene);
		static void Shutdown();
		static void UpdateWindowSize(float width, float height);

	private:
		static float GetUIScale();

		static float windowWidth;
		static float windowHeight;
	};
}