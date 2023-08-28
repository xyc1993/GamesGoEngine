#pragma once

#include "Material.h"

namespace GamesGoEngine
{
	class PostProcessMaterial : public Material
	{
	public:
		PostProcessMaterial();
		PostProcessMaterial(const GLchar* fragmentPath);

		void SetBlendWeight(float weight);
		float GetBlendWeight() const;

		// This method is used to determine if post process material should be used. In the future it could be expanded to handle local post process volumes.
		bool IsActive() const;

		virtual void Draw(glm::mat4 model) override;

		void SetPostProcessOrder(int orderValue);
		int GetPostProcessOrder() const;

	private:
		float blendWeight;
		int postProcessOrder;
	};
}