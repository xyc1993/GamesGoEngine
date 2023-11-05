#include "AssetPropertiesPanel.h"

#include <imgui.h>

#include "AssetMaterial.h"
#include "AssetShader.h"
#include "AssetsManager.h"
#include "AssetTexture.h"
#include "EditorUIUtils.h"

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
			std::string vertexShaderPath = materialAsset->GetVertexShaderPath();
			if (vertexShaderPath.empty())
			{
				// whitespaces just for displaying UI element
				vertexShaderPath = "                ";
			}
			vertexShaderPath.append("##vertex_shader_path");
			ImGui::Button(vertexShaderPath.c_str());
			// Try to retrieve shader
			vertexShaderPath = EditorUIUtils::TryGetDropTargetAssetPath(AssetType::Shader);
			if (!vertexShaderPath.empty())
			{
				materialAsset->SetVertexShaderPath(vertexShaderPath);
			}

			// Fragment shader input field
			ImGui::Text("FragmentShader");
			std::string fragmentShaderPath = materialAsset->GetFragmentShaderPath();
			if (fragmentShaderPath.empty())
			{
				// whitespaces just for displaying UI element
				fragmentShaderPath = "                ";
			}
			fragmentShaderPath.append("##fragment_shader_path");
			ImGui::Button(fragmentShaderPath.c_str());
			fragmentShaderPath = EditorUIUtils::TryGetDropTargetAssetPath(AssetType::Shader);
			if (!fragmentShaderPath.empty())
			{
				materialAsset->SetFragmentShaderPath(fragmentShaderPath);
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