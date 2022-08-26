#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
    vec2 texCoords;
} vs_out;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    vs_out.texCoords = aTexCoords;
    // projection matrix has to be applied after the vertices's positions are modified by geometry shader
    gl_Position = view * model * vec4(aPos, 1.0);
}