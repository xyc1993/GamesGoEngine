#version 330 core
out vec4 color;

uniform sampler2D mainTexture;

in vec2 TexCoord;

void main()
{
	vec4 texColor = texture(mainTexture, TexCoord);
	if (texColor.a < 0.1)
		discard;
	color = texColor;
}