#include "MeshBase.h"

void MeshBase::DrawSubMesh(size_t subMeshIndex) const
{
	if (subMeshIndex >= 0 && subMeshIndex < subMeshes.size())
	{
		if (subMeshes[subMeshIndex] != nullptr)
		{
			subMeshes[subMeshIndex]->Draw();
		}
	}
}

int MeshBase::GetSubMeshesCount() const
{
	return subMeshes.size();
}