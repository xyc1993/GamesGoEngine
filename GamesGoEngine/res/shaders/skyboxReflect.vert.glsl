#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

// layout (std140) statement means that the currently defined uniform block uses a specific memory layout for its content; this statement sets the uniform block layout.
// The general practice however is to not use the shared layout, but to use the std140 layout.
// The std140 layout explicitly states the memory layout for each variable type by standardizing their respective offsets governed by a set of rules
// 'binding = 0' is available from OpenGL version 4.2 and onwards
layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec3 Normal;
out vec3 Position;

uniform mat4 model;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(Position, 1.0);
}