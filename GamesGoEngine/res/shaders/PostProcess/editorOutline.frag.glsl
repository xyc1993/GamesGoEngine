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

    float stencilDifference = 0.0;
    for (int i = 0; i < 9; i++)
    {
        stencilDifference += abs(texture(depthStencilTexture, TexCoords.st + offsets[i]).r - texture(depthStencilTexture, TexCoords).r);
    }
    stencilDifference = clamp(stencilDifference, 0.0, 1.0);
    //stencilDifference = step(stencilDifference, 0.5);

    uint testStencilValue = texture(stencilView, TexCoords).r;
    float test = clamp(float(testStencilValue), 0.0, 1.0);

    vec3 col = texture(screenTexture, TexCoords).rgb;
    col += vec3(test);

    //vec3 finalColor = mix(col, outlineColor, stencilDifference);
    //vec3 finalColor = vec3(texture(screenTexture, TexCoords));

    //vec3 finalColor = vec3(float(testStencilValue));

    vec3 finalColor = mix(col, outlineColor, testStencilValue);

    FragColor = vec4(finalColor, 1.0);
}