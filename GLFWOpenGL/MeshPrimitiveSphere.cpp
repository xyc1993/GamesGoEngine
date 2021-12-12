#include "MeshPrimitiveSphere.h"

#include <iostream>

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
		for (int j = 0; j < longitudesNumber; j++)
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

			texCoords.x = (float)j / (float)(longitudesNumber - 1);
			texCoords.y = (float)i / (float)(latitudesNumber - 1);

			vertex.TexCoords = texCoords;

			//std::cout << vertices.size() << " : [" << j << "," << i << "] = <" << vertexPosition.x << ", " << vertexPosition.y << ", " << vertexPosition.z << ">" << std::endl;
			//std::cout << "[" << i << "/" << latitudesNumber - 1 << ", " << j << "/" << longitudesNumber - 1 << "] = <" << texCoords.x << ", " << texCoords.y << ">\n";

			vertices.push_back(vertex);
		}
	}
	
	// south pole indices
	for (int i = 0; i < longitudesNumber; i++)
	{
		indices.push_back(i);
		indices.push_back(i + longitudesNumber);

		if (i == (longitudesNumber - 1))
		{
			indices.push_back(i + 1);
		}
		else
		{
			indices.push_back(i + longitudesNumber + 1);
		}
	}
	
	// middle part
	for (int i = 0; i < latitudesNumber -1; i++)
	{
		for (int j = 0; j < longitudesNumber; j++)
		{
			if (j == (longitudesNumber - 1))
			{
				indices.push_back(j + longitudesNumber + i * longitudesNumber);
				indices.push_back(j + 2 * longitudesNumber + i * longitudesNumber);
				indices.push_back(j + 1 + i * longitudesNumber);

				indices.push_back(j + 2 * longitudesNumber + i * longitudesNumber);
				indices.push_back(j + 1 + longitudesNumber + i * longitudesNumber);
				indices.push_back(j + 1 + i * longitudesNumber);
			}
			else
			{
				indices.push_back(j + longitudesNumber + i * longitudesNumber);
				indices.push_back(j + 2 * longitudesNumber + i * longitudesNumber);
				indices.push_back(j + 1 + longitudesNumber + i * longitudesNumber);

				indices.push_back(j + 2 * longitudesNumber + i * longitudesNumber);
				indices.push_back(j + 2 * longitudesNumber + 1 + i * longitudesNumber);
				indices.push_back(j + 1 + longitudesNumber + i * longitudesNumber);
			}
		}
	}

	// north pole indices
	for (int i = 0; i < longitudesNumber; i++)
	{
		indices.push_back(vertices.size() - i - 1);
		indices.push_back(vertices.size() - i - 1 - longitudesNumber);

		if (i == 0)
		{
			indices.push_back(vertices.size() - i - 2 * longitudesNumber);
		}
		else
		{
			indices.push_back(vertices.size() - i - longitudesNumber);
		}
	}
	
	sphereSubMesh = new SubMesh("Sphere", vertices, indices);
}