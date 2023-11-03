#pragma once

#include <memory>

#include "Asset.h"
#include "MeshImported.h"

namespace GamesGoEngine
{
	class AssetMesh : public Asset
	{
	public:
		virtual void Load(AssetType type, std::string name, std::string path) override;

		std::shared_ptr<MeshImported> GetMesh() const;

	private:
		std::shared_ptr<MeshImported> mesh;
	};
}