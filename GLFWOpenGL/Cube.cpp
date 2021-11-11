#include "Cube.h"

#include "Primitives.h"
#include "TextureLoader.h"
#include <glm/gtc/type_ptr.hpp>

Cube::Cube()
{

}

Cube::Cube(Shader shader, glm::vec3 position, GLfloat angle, GLfloat scale)
{
    SetupCube();
    this->position = position;
    this->angle = angle;
    this->scale = scale;
    this->applyTextures = false;
}

Cube::Cube(GLchar* diffusePath, GLchar* specularPath, Shader shader, glm::vec3 position, GLfloat angle, GLfloat scale)
{
    SetupCube();
    ApplyTextures(diffusePath, specularPath, shader);
    this->position = position;
    this->angle = angle;
    this->scale = scale;
    this->applyTextures = true;
}

//not generic enough, details inside the method
void Cube::Draw(Shader shader)
{
    shader.Use();

    //this should be handled by dedicated 'Material' class!    
    if (applyTextures)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
    }

    //this could be handled by 'Renderer' class but think about what could be handled by dedicated 'Mesh'/'Model' class
    glBindVertexArray(VAO);
    
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f)); //random rotation axis for now
    model = glm::scale(model, glm::vec3(scale));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

//won't be relevant in the future, correct mesh renderer will read its owner data
void Cube::SetPosition(glm::vec3 position)
{
    this->position = position;
}

//could be more generic by passing the vertices with flags
void Cube::SetupCube()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

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

//could be more generic by taking (path, texture slot name, index and applying it to the shader)
void Cube::ApplyTextures(GLchar* diffusePath, GLchar* specularPath, Shader shader)
{
    diffuseMap = TextureLoader::LoadTexture(diffusePath);
    specularMap = TextureLoader::LoadTexture(specularPath);

    glBindTexture(GL_TEXTURE_2D, 0); //unbind textures

    shader.Use();
    glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shader.Program, "material.specular"), 1);
}