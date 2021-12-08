#pragma once

#include "MeshBase.h"

class MeshPrimitiveCube : public MeshBase
{
public:
	MeshPrimitiveCube();

	virtual void DrawSubMesh(size_t subMeshIndex) override;
	virtual int GetSubMeshesCount() const override;
};