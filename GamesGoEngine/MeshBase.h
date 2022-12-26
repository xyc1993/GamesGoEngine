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
	void CalculateTangents(glm::vec3& tangent, glm::vec3& bitangent,
		glm::vec3 trianglePos1, glm::vec3 trianglePos2, glm::vec3 trianglePos3,
		glm::vec2 triangleUV1, glm::vec2 triangleUV2, glm::vec2 triangleUV3);

	std::vector<SubMesh*> subMeshes;

private:
	bool shouldDrawInstanced = false;
	int instancesNumber = 0;
};