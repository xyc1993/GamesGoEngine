#version 420 core
in vec3 TexCoords;

out vec4 color;

layout(binding = 0) uniform samplerCube skybox;

void main()
{
	color = texture(skybox, TexCoords);
}