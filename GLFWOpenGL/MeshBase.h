#pragma once

#include "SubMesh.h"
#include "SharedAsset.h"

class MeshBase : public SharedAsset
{
public:
	virtual void DrawSubMesh(size_t subMeshIndex) const;
	virtual int GetSubMeshesCount() const;

protected:
	std::vector<SubMesh*> subMeshes;
};