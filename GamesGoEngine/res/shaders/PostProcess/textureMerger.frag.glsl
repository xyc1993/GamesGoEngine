#version 440
out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture1;
layout(binding = 1) uniform sampler2D screenTexture2;

uniform float blendWeight;

void main()
{
    vec3 col1 = texture(screenTexture1, TexCoords).rgb;
    vec3 col2 = texture(screenTexture2, TexCoords).rgb;
    vec3 finalColor = col1 + col2;
    FragColor = vec4(finalColor, 1.0);
} 