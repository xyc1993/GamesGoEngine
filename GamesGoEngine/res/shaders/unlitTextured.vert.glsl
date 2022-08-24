#version 420 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoord;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	//TexCoord = texCoord;
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y); //reversed for quads
};