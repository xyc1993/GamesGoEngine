#include "AssetMesh.h"

namespace GamesGoEngine
{
	void AssetMesh::Load(AssetType type, std::string name, std::string path)
	{
		Asset::Load(type, name, path);

		mesh = std::make_shared<MeshImported>(path);
	}

	std::shared_ptr<MeshImported> AssetMesh::GetMesh() const
	{
		return mesh;
	}
}