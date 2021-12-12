#include "MeshPrimitiveSphere.h"

SubMesh* MeshPrimitiveSphere::sphereSubMesh = nullptr;

MeshPrimitiveSphere::MeshPrimitiveSphere()
{
	Init();
}

SubMesh* MeshPrimitiveSphere::GetPrimitiveSubMesh()
{
	return sphereSubMesh;
}

void MeshPrimitiveSphere::SetupMesh()
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	
	const float longitudeAngleDelta = 2.0f * glm::pi<float>() / (float)longitudesNumber;
	const float latitudeAngleDelta = glm::pi<float>() / (float)(latitudesNumber - 1);

	// init vertex data first
	for (int i = 0; i < latitudesNumber; i++)
	{
		for (int j = 0; j <= longitudesNumber; j++)
		{
			Vertex vertex;

			glm::vec3 vertexPosition;

			float phi = j * longitudeAngleDelta;
			float theta = -0.5f * glm::pi<float>() + i * latitudeAngleDelta;

			vertexPosition.x = 0.5f * glm::cos(phi) * glm::cos(theta);
			vertexPosition.y = 0.5f * glm::sin(theta);
			vertexPosition.z = 0.5f * glm::sin(phi) * glm::cos(theta);

			vertex.Position = vertexPosition;

			glm::vec3 normal;
			normal = normalize(vertexPosition);
			vertex.Normal = normal;

			glm::vec2 texCoords;

			texCoords.x = (float)j / (float)longitudesNumber;
			texCoords.y = (float)i / (float)(latitudesNumber - 1);

			vertex.TexCoords = texCoords;
			
			vertices.push_back(vertex);
		}
	}
	
	// south pole indices
	for (GLuint i = 0; i < longitudesNumber; i++)
	{
		indices.push_back(i);
		indices.push_back(i + longitudesNumber + 1);
		indices.push_back(i + longitudesNumber + 2);
	}

	// middle part indices
	for (GLuint i = 0; i < latitudesNumber; i++)
	{
		for (int j = 0; j < longitudesNumber; j++)
		{
			indices.push_back(j + (i + 1) * (1 + longitudesNumber));
			indices.push_back(j + (i + 2) * (1 +  longitudesNumber));
			indices.push_back(j + (i + 1) * (1 + longitudesNumber) + 1);

			indices.push_back(j + (i + 2) * (1 + longitudesNumber));
			indices.push_back(j + (i + 2) * (1 + longitudesNumber) + 1);
			indices.push_back(j + (i + 1) * (1 + longitudesNumber) + 1);
		}
	}
	
	// north pole indices
	for (GLuint i = 0; i < longitudesNumber; i++)
	{
		indices.push_back(vertices.size() - 1 - i);
		indices.push_back(vertices.size() - 1 - i  - longitudesNumber - 1);
		indices.push_back(vertices.size() - 1 - i - longitudesNumber - 2);
	}
	
	sphereSubMesh = new SubMesh("Sphere", vertices, indices);
}