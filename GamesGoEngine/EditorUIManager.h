#pragma once

#include <string>

// forward declaration of window struct and enum to avoid linking libraries in this header
struct GLFWwindow;
namespace ImGuizmo { enum OPERATION : int; }

namespace GamesGoEngine
{
	enum class MsgType;
	class DebugToolsUI;
	class GraphicsSettingsUI;
	class LoggerUI;
	class PropertiesUI;
	class SceneViewport;
	class WorldOutlinerUI;

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
	private:
		void DrawPanels(const float windowWidth, const float windowHeight);
	public:
		static void Shutdown();
		static void SetTransformOperation(ImGuizmo::OPERATION transformOperation);
		static void SelectGameObjectAt(int x, int y);
		static void AddLog(MsgType type, std::string msg);
		static void ClearLogs();

	private:
		static float GetUIScale(const float& windowWidth, const float& windowHeight);

		DebugToolsUI* debugToolsUI;
		GraphicsSettingsUI* graphicsSettingsUI;
		LoggerUI* loggerUI;
		PropertiesUI* propertiesUI;
		SceneViewport* sceneViewer;
		WorldOutlinerUI* worldOutliner;
	};
}