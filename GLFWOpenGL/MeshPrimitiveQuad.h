#pragma once

#include "MeshPrimitiveBase.h"

class MeshPrimitiveQuad : public MeshPrimitiveBase
{
public:
	MeshPrimitiveQuad();

protected:
	virtual SubMesh* GetPrimitiveSubMesh() override;
	virtual void SetupMesh() override;

	// shared submesh among all of the cube instances, generated just once
	static SubMesh* quadSubMesh;
};