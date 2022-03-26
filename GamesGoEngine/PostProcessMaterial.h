#pragma once

#include "Material.h"

class PostProcessMaterial : public Material
{
public:
	PostProcessMaterial();
	PostProcessMaterial(const GLchar* fragmentPath);
};