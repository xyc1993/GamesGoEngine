#include "DebugToolsUI.h"

#include <string>

#include "imgui.h"
#include "RenderingManager.h"
#include "Time.h"

bool DebugToolsUI::wireframeOnly = false;

void DebugToolsUI::Draw()
{
	GLfloat fps = 1.0f / Time::GetUnscaledDeltaTime();
	std::string fpsText = "FPS = ";
	fpsText.append(std::to_string(fps));

	ImGui::Begin("Debug Tools");
	ImGui::Text(fpsText.c_str());
	float timeScale = Time::GetTimeScale();
	if (ImGui::SliderFloat("Time Scale", &timeScale, 0.0f, 5.0f))
	{
		Time::SetTimeScale(timeScale);
	}

	if (ImGui::Checkbox("Wireframe only", &wireframeOnly))
	{
		glPolygonMode(GL_FRONT_AND_BACK, wireframeOnly ? GL_LINE : GL_FILL);
		RenderingManager::EnablePostProcessing(!wireframeOnly);
	}
	
	ImGui::End();
}