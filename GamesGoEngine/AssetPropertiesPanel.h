#pragma once

#include "Asset.h"
#include "EditorPanel.h"

namespace GamesGoEngine
{
	class AssetMaterial;
	struct UniformData;

	class AssetPropertiesPanel : public EditorPanel
	{
	public:
		virtual void Draw() override;

	private:
		void DrawNameLabelField(Asset* asset) const;
		void TryDrawAssetMaterialData(Asset* asset) const;
		void DrawShaderUniforms(AssetMaterial* materialAsset, const std::map<std::string, UniformData>& uniformsMap) const;
		void TryDrawAssetShaderData(Asset* asset) const;
		void TryDrawAssetTextureData(Asset* asset) const;
	};
}