#pragma once

// forward declaration of window struct and enum to avoid linking libraries in this header
struct GLFWwindow;
namespace ImGuizmo { enum OPERATION : int; }

namespace GamesGoEngine
{
	class Scene;
	class SceneViewport;

	class EditorUIManager
	{
	private:
		EditorUIManager();
		~EditorUIManager();

		static EditorUIManager* instance;
		static EditorUIManager* GetInstance();

	public:
		static void Init(GLFWwindow* window);
		static void Draw();
		static void Shutdown();
		static void SetTransformOperation(ImGuizmo::OPERATION transformOperation);
		static void SelectGameObjectAt(int x, int y);

	private:
		static float GetUIScale(const float& windowWidth, const float& windowHeight);

		SceneViewport* sceneViewer;
	};
}