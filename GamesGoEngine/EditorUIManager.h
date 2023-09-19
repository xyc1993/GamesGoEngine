#pragma once

#include <string>
#include <vector>

// forward declaration of window struct and enum to avoid linking libraries in this header
struct GLFWwindow;
namespace ImGuizmo { enum OPERATION : int; }

namespace GamesGoEngine
{
	enum class MsgType;
	class EditorPanel;
	class DebugToolsUI;
	class EditorSettingsPanel;
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
		void DrawPanels();
	public:
		static void Shutdown();
		static void SetTransformOperation(ImGuizmo::OPERATION transformOperation);
		static void SelectGameObjectAt(int x, int y);
		static void AddLog(MsgType type, std::string msg);
		static void ClearLogs();

	private:
		DebugToolsUI* debugToolsUI;
		EditorSettingsPanel* editorSettingsPanel;
		GraphicsSettingsUI* graphicsSettingsUI;
		LoggerUI* loggerUI;
		PropertiesUI* propertiesUI;
		SceneViewport* sceneViewer;
		WorldOutlinerUI* worldOutliner;

		std::vector<EditorPanel*> editorPanels;
	};
}