#pragma once

#include "SubMesh.h"
#include "SharedAsset.h"

class MeshBase : public SharedAsset
{
public:
	virtual ~MeshBase();

	void DrawSubMesh() const;
	virtual void DrawSubMesh(size_t subMeshIndex) const;
	virtual int GetSubMeshesCount() const;

protected:
	std::vector<SubMesh*> subMeshes;
};