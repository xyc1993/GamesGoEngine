#pragma once

#include "MeshBase.h"

class CubePrimitive : public MeshBase
{
public:
	CubePrimitive();

	virtual void DrawSubmesh(size_t submeshIndex) override;
	virtual int GetSubmeshesCount() const override;
};