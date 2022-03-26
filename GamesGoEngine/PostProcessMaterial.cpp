#include "PostProcessMaterial.h"

PostProcessMaterial::PostProcessMaterial()
{
	shader = nullptr;
	texturesMap.clear();
	lightModelType = LightModelType::Unlit;
}

PostProcessMaterial::PostProcessMaterial(const GLchar* fragmentPath)
{
	shader = new Shader("res/shaders/PostProcess/screenShader.vert.glsl", fragmentPath);
	texturesMap.clear();
	lightModelType = LightModelType::Unlit;
}
