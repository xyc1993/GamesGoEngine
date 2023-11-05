#pragma once

#include <imgui.h>

#include "Asset.h"

namespace GamesGoEngine
{
	class AssetShader : public Asset
	{
	public:
		virtual void Load(AssetType type, std::string name, std::string path) override;
		const ImGuiTextBuffer& GetShaderTextBuffer() const;

	private:
		void WriteShaderTextToBuffer();

		ImGuiTextBuffer shaderTextBuffer;
	};
}