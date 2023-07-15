#include "TextureLoader.h"

GLuint TextureLoader::LoadTexture(GLchar* path, bool transparencyEnabled, bool sRGB)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);

    int imageWidth, imageHeight;

    unsigned char* image = SOIL_load_image(path, &imageWidth, &imageHeight, 0, transparencyEnabled ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

    const GLint internalFormat = sRGB ?
        transparencyEnabled ? GL_SRGB_ALPHA : GL_SRGB :
        transparencyEnabled ? GL_RGBA : GL_RGB;
    const GLint format = transparencyEnabled ? GL_RGBA : GL_RGB;

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, transparencyEnabled ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, transparencyEnabled ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    SOIL_free_image_data(image);

    return textureID;
}

GLuint TextureLoader::LoadCubemap(std::vector<const GLchar*> faces, bool sRGB)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int imageWidth, imageHeight;
    unsigned char* image;

    const GLint internalFormat = sRGB ? GL_SRGB : GL_RGB;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}