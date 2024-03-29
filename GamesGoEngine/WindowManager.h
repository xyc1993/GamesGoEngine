#pragma once

struct GLFWwindow;

namespace GamesGoEngine
{
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
		static int GetCurrentWidth();
		static int GetCurrentHeight();
		static float GetCurrentAspectRatio();
		static void EnableVSync(bool enable);
		static bool IsVSyncEnabled();

	private:
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		void UpdateAspectRatio();

		GLFWwindow* window;

		const int startWindowWidth = 1600;
		const int startWindowHeight = 900;

		int currentScreenWidth;
		int currentScreenHeight;
		float aspectRatio;

		bool vsyncEnabled;
	};
}