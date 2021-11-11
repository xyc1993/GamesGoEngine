#pragma once

#include "MeshBase.h"

class CubePrimitive : public MeshBase
{
public:
	CubePrimitive();

	void Draw() override;
};