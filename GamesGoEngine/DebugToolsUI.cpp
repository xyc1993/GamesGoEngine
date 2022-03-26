#include "DebugToolsUI.h"

#include <string>
#include <GLFW/glfw3.h>

#include "imgui.h"
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

	/* Due to framebuffer use in the rendering manager, this will not work and only display wireframe of the latest screen grab quad
	 * TODO: Find a way to display debug wireframe (perhaps disabling postprocess stack since for debug purposes it's not needed when checking the wireframe)
	if (ImGui::Checkbox("Wireframe only", &wireframeOnly))
	{
		glPolygonMode(GL_FRONT_AND_BACK, wireframeOnly ? GL_LINE : GL_FILL);
	}
	*/
	
	ImGui::End();
}