#include "DebugToolsUI.h"

#include <string>

#include "imgui.h"
#include "RenderingManager.h"
#include "Time.h"

void DebugToolsUI::Draw()
{
	ImGui::Begin("Debug Tools");

	GLfloat fps = 1.0f / Time::GetUnscaledDeltaTime();
	std::string fpsText = "FPS = ";
	fpsText.append(std::to_string(fps));
	ImGui::Text(fpsText.c_str());

	float timeScale = Time::GetTimeScale();
	if (ImGui::SliderFloat("Time Scale", &timeScale, 0.0f, 5.0f))
	{
		Time::SetTimeScale(timeScale);
	}

	bool wireframeOnly = RenderingManager::IsWireframeOnly();
	if (ImGui::Checkbox("Wireframe only", &wireframeOnly))
	{
		RenderingManager::SetWireframeOnly(wireframeOnly);
	}

	bool normalsDebugEnabled = RenderingManager::IsNormalsDebugEnabled();
	if (ImGui::Checkbox("Show normals", &normalsDebugEnabled))
	{
		RenderingManager::EnableNormalsDebug(normalsDebugEnabled);
	}
	
	ImGui::End();
}