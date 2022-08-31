#include "MeshBase.h"

MeshBase::~MeshBase()
{
	subMeshes.clear();
}

void MeshBase::DrawSubMesh() const
{
	DrawSubMesh(0);
}

void MeshBase::DrawSubMesh(size_t subMeshIndex) const
{
	if (subMeshIndex >= 0 && subMeshIndex < subMeshes.size())
	{
		if (subMeshes[subMeshIndex] != nullptr)
		{
			if (shouldDrawInstanced)
			{
				subMeshes[subMeshIndex]->DrawInstanced(instancesNumber);
			}
			else
			{
				subMeshes[subMeshIndex]->Draw();
			}
		}
	}
}

int MeshBase::GetSubMeshesCount() const
{
	return subMeshes.size();
}

bool MeshBase::IsImportedMesh() const
{
	return false;
}

SubMesh* MeshBase::GetSubMesh(int subMeshIndex) const
{
	if (subMeshIndex >= 0 && subMeshIndex < subMeshes.size())
	{
		return subMeshes[subMeshIndex];
	}
	return nullptr;
}

void MeshBase::SetInstancing(bool instancingEnabled, int numberOfInstances)
{
	shouldDrawInstanced = instancingEnabled;
	instancesNumber = numberOfInstances;
}
