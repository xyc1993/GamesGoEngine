#include "MeshPrimitiveQuad.h"

namespace GamesGoEngine
{
	SubMesh* MeshPrimitiveQuad::quadSubMesh = nullptr;

	MeshPrimitiveQuad::MeshPrimitiveQuad()
	{
		Init();
	}

	SubMesh* MeshPrimitiveQuad::GetPrimitiveSubMesh()
	{
		return quadSubMesh;
	}

	void MeshPrimitiveQuad::SetupMesh()
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		constexpr glm::vec3 quadPositions[] =
		{
			/* this is ok for regular objects scaling but for screen renderer it causes the framebuffer to be rendered at half width & height, therefore 1.0 instead of 0.5 is used
			glm::vec3(-0.5f, -0.5f,  0.0f),
			glm::vec3( 0.5f, -0.5f,  0.0f),
			glm::vec3( 0.5f,  0.5f,  0.0f),
			glm::vec3(-0.5f,  0.5f,  0.0f),
			*/

			glm::vec3(-1.0f, -1.0f,  0.0f),
			glm::vec3(1.0f, -1.0f,  0.0f),
			glm::vec3(1.0f,  1.0f,  0.0f),
			glm::vec3(-1.0f,  1.0f,  0.0f),
		};

		constexpr glm::vec3 quadNormal = glm::vec3(0.0f, 0.0f, 1.0f);

		constexpr glm::vec2 quadTexCoords[]
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};

		// calculate tangents, for quad only 1 tangent and 1 bitangent is needed
		glm::vec3 tangent, bitangent;
		CalculateTangents(tangent, bitangent,
			quadPositions[0], quadPositions[1], quadPositions[2],
			quadTexCoords[0], quadTexCoords[1], quadTexCoords[2]);

		// Set vertex and tangent data
		for (unsigned int i = 0; i < 4; i++)
		{
			Vertex vertex;
			vertex.Position = quadPositions[i];
			vertex.Normal = quadNormal;
			vertex.TexCoords = quadTexCoords[i];
			vertex.Tangent = tangent;
			vertex.Bitangent = bitangent;
			vertices.push_back(vertex);
		}

		constexpr unsigned int quadFaceIndices[] = { 0,1,2,2,3,0 };
		for (unsigned int i = 0; i < 6; i++)
		{
			indices.push_back(quadFaceIndices[i]);
		}

		//quadSubMesh = new SubMesh("Quad", vertices, indices, tangents);
		quadSubMesh = new SubMesh("Quad", vertices, indices);
	}
}