#include "MeshPrimitiveCube.h"

#include <iostream>

#include "Primitives.h"

//TO DO: cleanup where possible
MeshPrimitiveCube::MeshPrimitiveCube()
{
    subMeshes.clear();

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    glm::vec3 cubeVertices[] =
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

    glm::vec3 cubeNormals[] =
    {
        glm::vec3( 0.0f,  0.0f,  1.0f), // front
        glm::vec3( 0.0f,  0.0f, -1.0f), // back
        glm::vec3( 1.0f,  0.0f,  0.0f), // right
        glm::vec3(-1.0f,  0.0f,  0.0f), // left
        glm::vec3( 0.0f, -1.0f,  0.0f), // bottom
        glm::vec3( 0.0f,  1.0f,  0.0f)  // top
    };

    glm::vec2 cubeTexCoords[]
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    };
    
    GLuint cubeIndices[] =
    {
        // front
        0, 1, 2,
        2, 3, 0,

        // back
        4, 5, 6,
        6, 7, 4,

        // right
        8, 9, 10,
        10, 11, 8,

        // left
        12, 13, 14,
        14, 15, 12,

        // bottom
        16, 17, 18,
        18, 19, 16,

        // top
        20, 21, 22,
        22, 23, 20
    };

    GLuint cubeTexIndices[] = { 0,1,2,2,3,0 };

    for (GLuint i = 0; i < 24; i++)
    {
        Vertex vertex;
        vertex.Position = cubeVertices[i];
        vertex.Normal = cubeNormals[i / 4];
        vertex.TexCoords = cubeTexCoords[i % 4];
        vertices.push_back(vertex);
    }
    
    for (GLuint i = 0; i < 36; i++)
    {
        indices.push_back(cubeIndices[i]);
    }

    SubMesh* cubeSubMesh = new SubMesh("Cube", vertices, indices);
    subMeshes.push_back(cubeSubMesh);
}