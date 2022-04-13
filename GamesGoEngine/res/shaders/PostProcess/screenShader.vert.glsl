#version 440
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoords;

void main()
{
    TexCoords = texCoord;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}