#pragma once

#include "SubMesh.h"

class MeshBase
{
public:
	virtual ~MeshBase();

	void DrawSubMesh() const;
	virtual void DrawSubMesh(size_t subMeshIndex) const;
	virtual int GetSubMeshesCount() const;
	virtual bool IsImportedMesh() const;

	SubMesh* GetSubMesh(int subMeshIndex) const;
	void SetInstancing(bool instancingEnabled, int numberOfInstances = 0);

protected:
	std::vector<SubMesh*> subMeshes;

private:
	bool shouldDrawInstanced = false;
	int instancesNumber = 0;
};