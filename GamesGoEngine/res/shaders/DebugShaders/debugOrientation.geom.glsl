#version 420 core
layout(points) in;
layout(line_strip, max_vertices = 6) out;

const float MAGNITUDE = 2.0;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform vec3 forward;
uniform vec3 up;
uniform vec3 right;

out vec3 fColor;

void GenerateLine(vec3 direction, vec3 color)
{
    fColor = color;
    // projection matrix has to be applied after the vertices's positions are modified by geometry shader
    gl_Position = projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position +
        view * vec4(direction, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(forward, vec3(1.0f, 0.0f, 0.0f));
    GenerateLine(up, vec3(0.0f, 0.0f, 1.0f));
    GenerateLine(right, vec3(0.0f, 1.0f, 0.0f));
}