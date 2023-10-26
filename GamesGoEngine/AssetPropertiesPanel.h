#pragma once

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class Asset;

	class AssetPropertiesPanel : public EditorPanel
	{
	public:
		virtual void Draw() override;

	private:
		void DrawNameLabelField(Asset* asset) const;
		void TryDrawAssetMaterialData(Asset* asset) const;
		void TryDrawAssetTextureData(Asset* asset) const;
	};
}