#version 440
out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D depthStencilTexture;
layout(binding = 2) uniform usampler2D stencilView;

const vec3 outlineColor = vec3(1.0, 0.8, 0.0);
const float offset = 1.0 / 600.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2(0.0f, offset), // top-center
        vec2(offset, offset), // top-right
        vec2(-offset, 0.0f),   // center-left
        vec2(0.0f, 0.0f),   // center-center
        vec2(offset, 0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f, -offset), // bottom-center
        vec2(offset, -offset)  // bottom-right    
        );

    float stencilDifference = 0;
    for (int i = 0; i < 9; i++)
    {
        stencilDifference += abs(texture(stencilView, TexCoords.st + offsets[i]).r - texture(stencilView, TexCoords).r);
    }

    int colorLerp = clamp(int(stencilDifference), 0, 1);
    vec3 col = texture(screenTexture, TexCoords).rgb;
    vec3 finalColor = mix(col, outlineColor, stencilDifference);
    FragColor = vec4(finalColor, 1.0);
}