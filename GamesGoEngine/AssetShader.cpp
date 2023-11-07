#include "AssetShader.h"

#include <fstream>
#include <sstream>

#include "StringUtils.h"

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

	const std::map<std::string, std::string>& AssetShader::GetUniforms() const
	{
		return uniformsMap;
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
			TryGetUniformVariableData(line);
			shaderTextBuffer.appendf(line.c_str());
			shaderTextBuffer.appendf("\n");
		}
		shaderFile.close();
	}

	void AssetShader::TryGetUniformVariableData(std::string shaderLine)
	{
		// TODO: expand in the future to include different formatting and uniform buffers
		// For now we naively assume that all uniform format follow format "uniform {variable_type} {variable_name}"
		// In the future take into consideration formatting like "layout(...) uniform {variable_type} {variable_name}" and others
		if (StringUtils::IsWordsCountEqual(shaderLine, 3))
		{
			// Remove ';' from the shader line
			shaderLine.erase(remove(shaderLine.begin(), shaderLine.end(), ';'), shaderLine.end());

			std::istringstream iss(shaderLine);
			std::string uniform, uniformType, uniformName;
			iss >> uniform >> uniformType >> uniformName;
			
			// Make sure first word actually indicates uniform shader variable
			if (uniform == "uniform")
			{
				uniformsMap.emplace(uniformName, uniformType);
			}
		}
	}
}