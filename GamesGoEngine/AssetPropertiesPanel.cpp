#include "AssetPropertiesPanel.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AssetMaterial.h"
#include "AssetShader.h"
#include "AssetsManager.h"
#include "AssetTexture.h"
#include "EditorUIUtils.h"
#include "Material.h"

namespace GamesGoEngine
{
	void AssetPropertiesPanel::Draw()
	{
		ImGui::Begin("Asset Properties");

		Asset* selectedAsset = AssetsManager::GetSelectedAsset();
		if (selectedAsset != nullptr)
		{
			DrawNameLabelField(selectedAsset);

			switch(selectedAsset->GetType())
			{
			case AssetType::Material:
				ImGui::Text("Material");
				TryDrawAssetMaterialData(selectedAsset);
				break;
			case AssetType::Mesh:
				ImGui::Text("Mesh");
				break;
			case AssetType::Shader:
				ImGui::Text("Shader");
				TryDrawAssetShaderData(selectedAsset);
				break;
			case AssetType::Texture:
				ImGui::Text("Texture");
				TryDrawAssetTextureData(selectedAsset);
				break;
			case AssetType::Unsupported: 
				ImGui::Text("Unsupported");
				break;
			}
		}

		ImGui::End();
	}

	void AssetPropertiesPanel::DrawNameLabelField(Asset* asset) const
	{
		const std::string nameString = asset->GetName();
		static char nameBuffer[Asset::NAME_MAX_LENGTH];
		strcpy_s(nameBuffer, sizeof(nameBuffer), nameString.c_str());
		if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
		{
			asset->SetName(nameBuffer);
		}
	}

	void AssetPropertiesPanel::TryDrawAssetMaterialData(Asset* asset) const
	{
		if (AssetMaterial* materialAsset = dynamic_cast<AssetMaterial*>(asset))
		{
			// Vertex shader input field
			ImGui::Text("VertexShader");
			const std::string vertexShaderPath = materialAsset->GetVertexShaderPath();
			// Create button label based on shader path
			std::string vertexShaderPathLabel = vertexShaderPath;
			if (vertexShaderPathLabel.empty())
			{
				// whitespaces just for displaying UI element
				vertexShaderPathLabel = "                ";
			}
			vertexShaderPathLabel.append("##vertex_shader_path");
			ImGui::Button(vertexShaderPathLabel.c_str());
			// Try to retrieve shader from drag & drop target
			const std::string dropTargetVertexShaderPath = EditorUIUtils::TryGetDropTargetAssetPath(AssetType::Shader);
			if (!dropTargetVertexShaderPath.empty())
			{
				materialAsset->SetVertexShaderPath(dropTargetVertexShaderPath);
			}

			// Fragment shader input field
			ImGui::Text("FragmentShader");
			const std::string fragmentShaderPath = materialAsset->GetFragmentShaderPath();
			// Create button label based on shader path
			std::string fragmentShaderPathLabel = fragmentShaderPath;
			if (fragmentShaderPathLabel.empty())
			{
				// whitespaces just for displaying UI element
				fragmentShaderPathLabel = "                ";
			}
			fragmentShaderPathLabel.append("##fragment_shader_path");
			ImGui::Button(fragmentShaderPathLabel.c_str());
			// Try to retrieve shader from drag & drop target
			const std::string dropTargetFragmentShaderPath = EditorUIUtils::TryGetDropTargetAssetPath(AssetType::Shader);
			if (!dropTargetFragmentShaderPath.empty())
			{
				materialAsset->SetFragmentShaderPath(dropTargetFragmentShaderPath);
			}

			// Draw all of the shaders' uniforms' fields
			AssetShader* vertexShaderAsset = dynamic_cast<AssetShader*>(AssetsManager::GetAsset(vertexShaderPath));
			AssetShader* fragmentShaderAsset = dynamic_cast<AssetShader*>(AssetsManager::GetAsset(fragmentShaderPath));
			if (vertexShaderAsset != nullptr && fragmentShaderAsset != nullptr)
			{
				if (!vertexShaderAsset->GetUniforms().empty() || !fragmentShaderAsset->GetUniforms().empty())
				{
					ImGui::Text("Material uniforms");
				}

				DrawShaderUniforms(materialAsset, vertexShaderAsset->GetUniforms());
				DrawShaderUniforms(materialAsset, fragmentShaderAsset->GetUniforms());
			}			
		}
	}

	void AssetPropertiesPanel::DrawShaderUniforms(AssetMaterial* materialAsset, const std::map<std::string, UniformData>& uniformsMap) const
	{
		if (materialAsset != nullptr)
		{
			for (auto uniform = uniformsMap.begin(); uniform != uniformsMap.end(); ++uniform)
			{
				std::string uniformName = uniform->first;
				std::string uniformType = uniform->second.uniformType;

				// TODO: add more types
				if (uniformType == "int")
				{
					int intField = materialAsset->GetMaterial()->GetInt(uniformName.c_str());
					if (ImGui::DragInt(uniformName.c_str(), &intField))
					{
						materialAsset->GetMaterial()->SetFloat(uniformName.c_str(), intField);
					}
				}

				if (uniformType == "float")
				{
					float floatField = materialAsset->GetMaterial()->GetFloat(uniformName.c_str());
					if (ImGui::DragFloat(uniformName.c_str(), &floatField, 0.1f))
					{
						materialAsset->GetMaterial()->SetFloat(uniformName.c_str(), floatField);
					}
				}

				if (uniformType == "vec2")
				{
					glm::vec2 vec2Field = materialAsset->GetMaterial()->GetVector2(uniformName.c_str());
					if (ImGui::DragFloat2(uniformName.c_str(), glm::value_ptr(vec2Field), 0.1f))
					{
						materialAsset->GetMaterial()->SetVector2(uniformName.c_str(), vec2Field);
					}
				}

				if (uniformType == "vec3")
				{
					glm::vec3 vec3Field = materialAsset->GetMaterial()->GetVector3(uniformName.c_str());
					if (ImGui::DragFloat3(uniformName.c_str(), glm::value_ptr(vec3Field), 0.1f))
					{
						materialAsset->GetMaterial()->SetVector3(uniformName.c_str(), vec3Field);
					}
				}

				if (uniformType == "vec4")
				{
					glm::vec4 vec4Field = materialAsset->GetMaterial()->GetVector4(uniformName.c_str());
					if (ImGui::DragFloat4(uniformName.c_str(), glm::value_ptr(vec4Field), 0.1f))
					{
						materialAsset->GetMaterial()->SetVector3(uniformName.c_str(), vec4Field);
					}
				}

				if (uniformType == "sampler2D")
				{
					std::string textureFieldLabel = "Texture ";
					textureFieldLabel.append(uniformName);
					ImGui::Text(textureFieldLabel.c_str());
					ImGui::SameLine();

					const std::string textureAssetPath = materialAsset->GetMaterial()->GetTexturePath(uniformName);
					// Create button label based on shader path
					std::string texturePathLabel = textureAssetPath;
					if (texturePathLabel.empty())
					{
						// whitespaces just for displaying UI element
						texturePathLabel = "                ";
					}
					texturePathLabel.append("##texture_path_");
					texturePathLabel.append(uniformName);
					ImGui::Button(texturePathLabel.c_str());
					// Try to retrieve texture from drag & drop target
					std::string dropTargetTexturePath = EditorUIUtils::TryGetDropTargetAssetPath(AssetType::Texture);
					AssetTexture* textureAsset = dynamic_cast<AssetTexture*>(AssetsManager::GetAsset(dropTargetTexturePath));
					if (!dropTargetTexturePath.empty() && (textureAsset != nullptr))
					{
						int textureIndex = uniform->second.binding;
						materialAsset->GetMaterial()->SetTextureByPath(uniformName.c_str(), textureIndex,
							dropTargetTexturePath.c_str(), textureAsset->IsTransparencyEnabled(), textureAsset->IsSRGB());
					}
				}
			}
		}
	}

	void AssetPropertiesPanel::TryDrawAssetShaderData(Asset* asset) const
	{
		if (AssetShader* shaderAsset = dynamic_cast<AssetShader*>(asset))
		{
			ImGui::TextUnformatted(shaderAsset->GetShaderTextBuffer().begin(), shaderAsset->GetShaderTextBuffer().end());
		}
	}

	void AssetPropertiesPanel::TryDrawAssetTextureData(Asset* asset) const
	{
		if (AssetTexture* textureAsset = dynamic_cast<AssetTexture*>(asset))
		{
			bool transparencyEnabled = textureAsset->IsTransparencyEnabled();
			if (ImGui::Checkbox("Transparency enabled", &transparencyEnabled))
			{
				textureAsset->EnableTransparency(transparencyEnabled);
			}

			bool isSRGB = textureAsset->IsSRGB();
			if (ImGui::Checkbox("sRGB", &isSRGB))
			{
				textureAsset->EnableSRGB(isSRGB);
			}

			ImGui::BeginChild("Texture Image");

			// texture dimensions
			const float width = static_cast<float>(textureAsset->GetWidth());
			const float height = static_cast<float>(textureAsset->GetHeight());

			// image view dimensions, get width based on panel size and scale height based on aspect ratio of the texture
			const float viewWidth = ImGui::GetWindowSize().x;
			const float viewHeight = height / width * viewWidth;

			const ImVec2 imageSize = ImVec2(viewWidth, viewHeight);
			ImGui::Image((void*)textureAsset->GetTexture(), imageSize);

			ImGui::EndChild();

			std::string imageResolutionString;
			imageResolutionString.append(std::to_string(textureAsset->GetWidth()));
			imageResolutionString.append("x");
			imageResolutionString.append(std::to_string(textureAsset->GetHeight()));
			ImGui::Text(imageResolutionString.c_str());
		}
	}
}
