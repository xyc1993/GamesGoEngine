#include "MeshPrimitiveBase.h"

void MeshPrimitiveBase::Init()
{
	subMeshes.clear();
	if (GetPrimitiveSubMesh() == nullptr)
	{
		SetupMesh();
	}
	subMeshes.push_back(GetPrimitiveSubMesh());
}