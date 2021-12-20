#include "MeshPrimitiveCube.h"

SubMesh* MeshPrimitiveCube::cubeSubMesh = nullptr;

MeshPrimitiveCube::MeshPrimitiveCube()
{
    Init();
}

SubMesh* MeshPrimitiveCube::GetPrimitiveSubMesh()
{
	return cubeSubMesh;
}

void MeshPrimitiveCube::SetupMesh()
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    constexpr glm::vec3 cubeVertices[] =
    {
        // front
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),

        // back
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),

        // right
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),

        // left
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),

        // bottom
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),

        // top
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
    };

    constexpr glm::vec3 cubeNormals[] =
    {
        glm::vec3( 0.0f,  0.0f,  1.0f), // front
        glm::vec3( 0.0f,  0.0f, -1.0f), // back
        glm::vec3( 1.0f,  0.0f,  0.0f), // right
        glm::vec3(-1.0f,  0.0f,  0.0f), // left
        glm::vec3( 0.0f, -1.0f,  0.0f), // bottom
        glm::vec3( 0.0f,  1.0f,  0.0f)  // top
    };

    constexpr glm::vec2 cubeTexCoords[]
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    };
    
    for (GLuint i = 0; i < 24; i++)
    {
        Vertex vertex;
        vertex.Position = cubeVertices[i];
        vertex.Normal = cubeNormals[i / 4];
        vertex.TexCoords = cubeTexCoords[i % 4];
        vertices.push_back(vertex);
    }

    // indices for the single face, repeat for every other face with proper index offset
    constexpr GLuint cubeFaceIndices[] = { 0,1,2,2,3,0 };
    for (GLuint i = 0; i < 6; i++)
    {
        indices.push_back(cubeFaceIndices[0] + 4 * i);
        indices.push_back(cubeFaceIndices[1] + 4 * i);
        indices.push_back(cubeFaceIndices[2] + 4 * i);
        indices.push_back(cubeFaceIndices[3] + 4 * i);
        indices.push_back(cubeFaceIndices[4] + 4 * i);
        indices.push_back(cubeFaceIndices[5] + 4 * i);
    }

    cubeSubMesh = new SubMesh("Cube", vertices, indices);
}