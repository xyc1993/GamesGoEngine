#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out VS_OUT{
    vec3 normal;
} vs_out;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    // projection matrix has to be applied after the vertices's positions are modified by geometry shader
    gl_Position = view * model * vec4(aPos, 1.0);
    // normal matrix is necessary to take into account rotations and scaling of the model
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}