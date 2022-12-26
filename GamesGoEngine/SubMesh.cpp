#include "SubMesh.h"

SubMesh::SubMesh(std::string name, std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
    this->name = name;
	this->vertices = vertices;
	this->indices = indices;
    this->SetupMesh();
}

SubMesh::SubMesh(std::string name, std::vector<Vertex> vertices, std::vector<GLuint> indices,
	std::vector<Tangents> tangents)
{
    this->name = name;
    this->vertices = vertices;
    this->indices = indices;
    this->tangents = tangents;
    this->SetupMesh();
}

void SubMesh::Draw()
{
    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SubMesh::DrawInstanced(int meshCount)
{
    glBindVertexArray(this->VAO);
    glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, meshCount);
    glBindVertexArray(0);
}

std::string SubMesh::GetName() const
{
    return name;
}

GLuint SubMesh::GetVAO() const
{
    return VAO;
}

void SubMesh::SetupMesh()
{
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers    
    const int bufferSize = tangents.empty() ? sizeof(Vertex) : sizeof(Vertex) + sizeof(Tangents);
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferSize, (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, bufferSize, (GLvoid*)(3 * sizeof(float)));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, bufferSize, (GLvoid*)(6 * sizeof(float)));

    if (!tangents.empty())
    {
        // Tangents
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, bufferSize, (GLvoid*)(8 * sizeof(float)));
        // Bitangents
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, bufferSize, (GLvoid*)(11 * sizeof(float)));
    }

    glBindVertexArray(0);
}
