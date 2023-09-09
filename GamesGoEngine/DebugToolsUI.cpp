#include "DebugToolsUI.h"

#include <string>

#include "imgui.h"
#include "RenderingManager.h"
#include "Time.h"

namespace GamesGoEngine
{
	DebugToolsUI::DebugToolsUI()
	{

	}

	void DebugToolsUI::Draw()
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("Debug Tools", nullptr, windowFlags);

		GLfloat fps = 1.0f / Time::GetUnscaledDeltaTime();
		std::string fpsText = "FPS = ";
		fpsText.append(std::to_string(fps));
		ImGui::Text(fpsText.c_str());

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