#version 440
out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D depthStencilTexture;
layout(binding = 2) uniform usampler2D stencilView;

uniform float blendWeight;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    vec3 invCol = vec3(1.0 - col);
    vec3 finalColor = mix(col, invCol, blendWeight);
    FragColor = vec4(finalColor, 1.0);
}