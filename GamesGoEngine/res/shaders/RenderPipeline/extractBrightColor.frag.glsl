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

    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(col, vec3(0.2126, 0.7152, 0.0722));

    if(brightness > 1.0)
        FragColor = vec4(col, 1.0);
    else
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}