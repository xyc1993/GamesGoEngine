#include "MeshPrimitiveQuad.h"

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
	std::vector<GLuint> indices;

	constexpr glm::vec3 quadVertices[] =
	{
		glm::vec3(-0.5f, -0.5f,  0.0f),
		glm::vec3( 0.5f, -0.5f,  0.0f),
		glm::vec3( 0.5f,  0.5f,  0.0f),
		glm::vec3(-0.5f,  0.5f,  0.0f),
	};

	constexpr glm::vec3 quadNormal = glm::vec3(0.0f, 0.0f, 1.0f);

	constexpr glm::vec2 quadTexCoords[]
	{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};

	for (GLuint i = 0; i < 4; i++)
	{
		Vertex vertex;
		vertex.Position = quadVertices[i];
		vertex.Normal = quadNormal;
		vertex.TexCoords = quadTexCoords[i];
		vertices.push_back(vertex);
	}

	constexpr GLuint quadFaceIndices[] = { 0,1,2,2,3,0 };
	for (GLuint i = 0; i < 6; i++)
	{
		indices.push_back(quadFaceIndices[i]);
	}

	quadSubMesh = new SubMesh("Quad", vertices, indices);
}