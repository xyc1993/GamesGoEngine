#pragma once

#include "SubMesh.h"

class MeshBase
{
public:
	virtual ~MeshBase();

	void DrawSubMesh() const;
	virtual void DrawSubMesh(size_t subMeshIndex) const;
	virtual int GetSubMeshesCount() const;

protected:
	std::vector<SubMesh*> subMeshes;
};