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

	const std::map<std::string, UniformData>& AssetShader::GetUniforms() const
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
			// Remove unwanted special characters from the shader line
			shaderLine.erase(remove(shaderLine.begin(), shaderLine.end(), ';'), shaderLine.end());
			shaderLine.erase(remove(shaderLine.begin(), shaderLine.end(), '('), shaderLine.end());
			shaderLine.erase(remove(shaderLine.begin(), shaderLine.end(), ')'), shaderLine.end());

			std::istringstream iss(shaderLine);

			std::vector<std::string> words;
			std::string word;

			// TODO: change it to be cleaner and more universal, right now it doesn't take into consideration lack of whitespaces that can occur in shaders
			// assume parsing like (binding = 0)
			int binding = 0;
			bool previousWordWasBinding = false;
			bool nextWordWillBeBinding = false;

			while (iss >> word)
			{
				if (nextWordWillBeBinding)
				{
					// TODO: check if conversion to integer is even possible
					binding = std::stoi(word);
					nextWordWillBeBinding = false;
				}

				if (previousWordWasBinding)
				{
					previousWordWasBinding = false;
					nextWordWillBeBinding = true;
				}

				if (word == "binding")
				{
					previousWordWasBinding = true;
				}

				words.push_back(word);
			}

			std::string uniformName = words[words.size() - 1];
			std::string uniformType = words[words.size() - 2];

			UniformData data;
			data.uniformName = uniformName;
			data.uniformType = uniformType;
			data.binding = binding;
			
			uniformsMap.emplace(uniformName, data);
		}
	}
}