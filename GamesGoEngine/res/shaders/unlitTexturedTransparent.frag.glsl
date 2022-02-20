#version 330 core
out vec4 color;

uniform sampler2D mainTexture;

in vec2 TexCoord;

void main()
{
	color = texture(mainTexture, TexCoord);
}