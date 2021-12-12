#pragma once

#include "MeshBase.h"

class MeshPrimitiveCube : public MeshBase
{
public:
	MeshPrimitiveCube();

private:
	static void SetupMesh();

	// shared submesh among all of the cube instances, generated just once
	static SubMesh* cubeSubMesh;
};