#include "MeshPrimitiveCube.h"

#include <iostream>

#include "Primitives.h"

MeshPrimitiveCube::MeshPrimitiveCube()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Primitives::cubeVertices), Primitives::cubeVertices, GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

void MeshPrimitiveCube::DrawSubMesh(size_t subMeshIndex)
{
    if (subMeshIndex != 0)
    {
        std::cout << "CUBE PRIMITIVE :: TRYING TO DRAW NON EXISTING SUBMESH!" << std::endl;
        return;
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

int MeshPrimitiveCube::GetSubMeshesCount() const
{
    // there's only 1 submesh in cube primitive - the cube
    return 1;
}