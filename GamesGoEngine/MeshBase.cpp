#include "MeshBase.h"

MeshBase::~MeshBase()
{
	subMeshes.clear();
}

void MeshBase::DrawSubMesh() const
{
	DrawSubMesh(0);
}

void MeshBase::DrawSubMesh(size_t subMeshIndex) const
{
	if (subMeshIndex >= 0 && subMeshIndex < subMeshes.size())
	{
		if (subMeshes[subMeshIndex] != nullptr)
		{
			if (shouldDrawInstanced)
			{
				subMeshes[subMeshIndex]->DrawInstanced(instancesNumber);
			}
			else
			{
				subMeshes[subMeshIndex]->Draw();
			}
		}
	}
}

int MeshBase::GetSubMeshesCount() const
{
	return subMeshes.size();
}

bool MeshBase::IsImportedMesh() const
{
	return false;
}

SubMesh* MeshBase::GetSubMesh(int subMeshIndex) const
{
	if (subMeshIndex >= 0 && subMeshIndex < subMeshes.size())
	{
		return subMeshes[subMeshIndex];
	}
	return nullptr;
}

void MeshBase::SetInstancing(bool instancingEnabled, int numberOfInstances)
{
	shouldDrawInstanced = instancingEnabled;
	instancesNumber = numberOfInstances;
}

void MeshBase::CalculateTangents(glm::vec3& tangent, glm::vec3& bitangent, glm::vec3 trianglePos1,
	glm::vec3 trianglePos2, glm::vec3 trianglePos3, glm::vec2 triangleUV1, glm::vec2 triangleUV2, glm::vec2 triangleUV3)
{
	glm::vec3 edge1 = trianglePos2 - trianglePos1;
	glm::vec3 edge2 = trianglePos3 - trianglePos1;
	glm::vec2 deltaUV1 = triangleUV2 - triangleUV1;
	glm::vec2 deltaUV2 = triangleUV3 - triangleUV1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	tangent = normalize(tangent);
	bitangent = normalize(bitangent);
}