#version 420 core
layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

uniform float protrusion;

void main()
{
	gl_Position = projection * view * model * vec4( position, 1.0) + protrusion * vec4( normal, 1.0);
};