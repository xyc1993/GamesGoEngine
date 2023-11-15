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
		// TODO: expand in the future to include different formatting and uniform buffers when implementing Vulkan
		if (StringUtils::Contains(shaderLine, "uniform") && StringUtils::GetWordsCount(shaderLine) >= 3)
		{
			// Remove ';' from the shader line
			shaderLine.erase(remove(shaderLine.begin(), shaderLine.end(), ';'), shaderLine.end());

			std::istringstream iss(shaderLine);

			std::vector<std::string> words;
			std::string word;
			while (iss >> word)
			{
				words.push_back(word);
			}

			std::string uniformType = words[words.size() - 2];
			std::string uniformName = words[words.size() - 1];
			
			uniformsMap.emplace(uniformName, uniformType);
		}
	}
}