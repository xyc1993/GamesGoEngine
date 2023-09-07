#pragma once

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

namespace GamesGoEngine
{
	class Scene;

	class EditorUIManager
	{
	private:
		EditorUIManager();

		static EditorUIManager* instance;
		static EditorUIManager* GetInstance();

	public:
		static void Init(GLFWwindow* window);
		static void Draw(Scene* activeScene);
		static void Shutdown();
		static float GetViewportPosX();
		static float GetViewportPosY();
		static float GetViewportWidth();
		static float GetViewportHeight();
		static void UpdateViewportDimensions();
		static void UpdateWindowSize(float width, float height);
		static void SetTransformOperation(ImGuizmo::OPERATION transformOperation);

	private:
		static float GetUIScale(const float& windowWidth, const float& windowHeight);

		float windowWidth;
		float windowHeight;
		float viewportPanelPosX;
		float viewportPanelPosY;
		float viewportPanelWidth;
		float viewportPanelHeight;
		float viewportTextureWidth;
		float viewportTextureHeight;
		ImGuizmo::OPERATION currentTransformOperation;
	};
}