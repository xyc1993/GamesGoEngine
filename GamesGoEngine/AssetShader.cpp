#include "AssetShader.h"

#include <fstream>

namespace GamesGoEngine
{
	void AssetShader::Load(AssetType type, std::string name, std::string path)
	{
		Asset::Load(type, name, path);

		WriteShaderTextToBuffer();
		// TODO: get meta data regarding uniforms (needed so that material properties panel knows what can be edited)
	}

	const ImGuiTextBuffer& AssetShader::GetShaderTextBuffer() const
	{
		return shaderTextBuffer;
	}

	void AssetShader::WriteShaderTextToBuffer()
	{
		// Clear buffer first to ensure no leftover data
		shaderTextBuffer.clear();

		// Open file to read text line by line
		std::ifstream shaderFile(path);
		std::string line;
		while (std::getline(shaderFile, line))
		{
			shaderTextBuffer.appendf(line.c_str());
			shaderTextBuffer.appendf("\n");
		}
		shaderFile.close();
	}
}