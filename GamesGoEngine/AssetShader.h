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
		const std::map<std::string, std::string>& GetUniforms() const;

	private:
		void WriteShaderTextToBuffer();
		void TryGetUniformVariableData(std::string shaderLine);

		ImGuiTextBuffer shaderTextBuffer;
		// key is name, value is type
		std::map<std::string, std::string> uniformsMap;
	};
}