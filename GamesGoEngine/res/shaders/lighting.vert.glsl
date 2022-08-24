#version 420 core
layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texCoord;

out vec3 Normal;
out vec3 FragPos;
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
	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(position, 1.0));	
	TexCoord = texCoord;
	//TexCoord = vec2(texCoord.x, 1.0 - texCoord.y); //top Y axis is inverted with OpenGL
};