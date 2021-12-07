#pragma once

#include "MeshBase.h"

class MeshPrimitiveCube : public MeshBase
{
public:
	MeshPrimitiveCube();

	virtual void DrawSubmesh(size_t submeshIndex) override;
	virtual int GetSubmeshesCount() const override;
};