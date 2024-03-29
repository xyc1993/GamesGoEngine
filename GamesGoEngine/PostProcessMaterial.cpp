#include "PostProcessMaterial.h"

#include <glm/gtc/type_ptr.hpp>

#include "Math.h"
#include "RenderingManager.h"

namespace GamesGoEngine
{
	PostProcessMaterial::PostProcessMaterial()
	{
		shader = nullptr;
		texturesMap.clear();
		lightModelType = LightModelType::Unlit;
	}

	PostProcessMaterial::PostProcessMaterial(const char* fragmentPath)
	{
		shader = new Shader("res/shaders/PostProcess/screenShader.vert.glsl", fragmentPath);
		texturesMap.clear();
		lightModelType = LightModelType::Unlit;
		SetBlendWeight(1.0f);
	}

	void PostProcessMaterial::SetBlendWeight(float weight)
	{
		blendWeight = weight;
		SetFloat("blendWeight", blendWeight);
	}

	float PostProcessMaterial::GetBlendWeight() const
	{
		return blendWeight;
	}

	bool PostProcessMaterial::IsActive() const
	{
		return !Math::IsNearlyZero(blendWeight);
	}

	void PostProcessMaterial::Draw(glm::mat4 model)
	{
		if (shader == nullptr)
		{
			return;
		}

		shader->Use();

		for (auto it = texturesMap.begin(); it != texturesMap.end(); ++it)
		{
			glActiveTexture(GL_TEXTURE0 + std::get<0>(it->second));
			glBindTexture(GL_TEXTURE_2D, std::get<1>(it->second));
			glUniform1i(it->first, std::get<0>(it->second));

			if (std::get<0>(it->second) == 0)
			{
				// do nothing for color texture
			}

			if (std::get<0>(it->second) == 1)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
			}

			if (std::get<0>(it->second) == 2)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
		}

		// post processes do not support cube textures
		for (auto it = cubeTexturesMap.begin(); it != cubeTexturesMap.end(); ++it)
		{
			glActiveTexture(GL_TEXTURE0 + std::get<0>(it->second));
			glBindTexture(GL_TEXTURE_CUBE_MAP, std::get<1>(it->second));
		}

		const GLint modelLoc = glGetUniformLocation(shader->GetProgram(), "model");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	}

	void PostProcessMaterial::SetPostProcessOrder(int orderValue)
	{
		postProcessOrder = orderValue;
		RenderingManager::SortPostProcessMaterials();
	}

	int PostProcessMaterial::GetPostProcessOrder() const
	{
		return postProcessOrder;
	}
}