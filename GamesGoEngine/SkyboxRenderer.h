#pragma once

#include "Renderer.h"

namespace GamesGoEngine
{
	class SkyboxRenderer : public Renderer
	{
	public:
		SkyboxRenderer();

		virtual void Draw() override;
		void SetMaterial(const std::shared_ptr<Material>& material);

	protected:
		virtual void InitMetaData() override;
	};
}