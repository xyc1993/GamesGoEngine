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
	glm::vec3 tangent;
	glm::vec3 bitangent;

	glm::vec3 edge1 = quadVertices[1] - quadVertices[0];
	glm::vec3 edge2 = quadVertices[2] - quadVertices[0];
	glm::vec2 deltaUV1 = quadTexCoords[1] - quadTexCoords[0];
	glm::vec2 deltaUV2 = quadTexCoords[2] - quadTexCoords[0];

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	tangent = normalize(tangent);
	bitangent = normalize(bitangent);

	// Set vertex and tangent data
	for (GLuint i = 0; i < 4; i++)
	{
		Vertex vertex;
		vertex.Position = quadVertices[i];
		vertex.Normal = quadNormal;
		vertex.TexCoords = quadTexCoords[i];
		vertex.Tangent = tangent;
		vertex.Bitangent = bitangent;
		vertices.push_back(vertex);
	}

	constexpr GLuint quadFaceIndices[] = { 0,1,2,2,3,0 };
	for (GLuint i = 0; i < 6; i++)
	{
		indices.push_back(quadFaceIndices[i]);
	}

	//quadSubMesh = new SubMesh("Quad", vertices, indices, tangents);
	quadSubMesh = new SubMesh("Quad", vertices, indices);
}