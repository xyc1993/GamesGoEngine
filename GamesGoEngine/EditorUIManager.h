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
	class DebugToolsPanel;
	class EditorSettingsPanel;
	class GraphicsSettingsPanel;
	class LoggerPanel;
	class PropertiesPanel;
	class SceneViewportPanel;
	class WorldOutlinerPanel;

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
		static void OnFrameDrawBegin();
		static void OnFrameDrawEnd();
		void DrawPanels();
	public:
		static void Shutdown();
		static void SetTransformOperation(ImGuizmo::OPERATION transformOperation);
		static void SelectGameObjectAt(int x, int y);
		static void AddLog(MsgType type, std::string msg);
		static void ClearLogs();

	private:
		DebugToolsPanel* debugToolsPanel;
		EditorSettingsPanel* editorSettingsPanel;
		GraphicsSettingsPanel* graphicsSettingsPanel;
		LoggerPanel* loggerPanel;
		PropertiesPanel* propertiesPanel;
		SceneViewportPanel* sceneViewerPanel;
		WorldOutlinerPanel* worldOutlinerPanel;

		std::vector<EditorPanel*> editorPanels;
	};
}