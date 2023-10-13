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
		void TryDrawAssetTextureData(Asset* asset) const;
	};
}