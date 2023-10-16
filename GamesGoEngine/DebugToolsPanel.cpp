#include "DebugToolsPanel.h"

#include <string>
#include <imgui.h>

#include "RenderingManager.h"
#include "Time.h"

namespace GamesGoEngine
{
	DebugToolsPanel::DebugToolsPanel()
	{

	}

	void DebugToolsPanel::Draw()
	{
		ImGui::Begin("Debug Tools");

		GLfloat fps = 1.0f / Time::GetUnscaledDeltaTime();
		std::string fpsText = "FPS = ";
		fpsText.append(std::to_string(fps));
		ImGui::Text(fpsText.c_str());

		GLfloat milliseconds = Time::GetUnscaledDeltaTime() * 1000.0f;
		std::string millisecondsText = "Time per frame = ";
		millisecondsText.append(std::to_string(milliseconds));
		millisecondsText.append(" [ms]");
		ImGui::Text(millisecondsText.c_str());

		float timeScale = Time::GetTimeScale();
		if (ImGui::SliderFloat("Time Scale", &timeScale, 0.0f, 5.0f))
		{
			Time::SetTimeScale(timeScale);
		}

		/* Currently in order to support this view, some changes would need to be applied in the rendering manager: disable all screen processing to quad
		 * TODO: enable once wireframe is working again
		bool wireframeOnly = RenderingManager::IsWireframeOnly();
		if (ImGui::Checkbox("Wireframe only", &wireframeOnly))
		{
			RenderingManager::SetWireframeOnly(wireframeOnly);
		}
		*/

		bool normalsDebugEnabled = RenderingManager::IsNormalsDebugEnabled();
		if (ImGui::Checkbox("Show normals", &normalsDebugEnabled))
		{
			RenderingManager::EnableNormalsDebug(normalsDebugEnabled);
		}

		ImGui::End();
	}
}