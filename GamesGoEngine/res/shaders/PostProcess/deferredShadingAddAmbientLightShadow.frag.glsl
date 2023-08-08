#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D gAlbedo;
layout(binding = 2) uniform sampler2D gLightEnabled;
layout(binding = 3) uniform sampler2D ssao;

uniform float ambientLightActive;
uniform vec3 ambientLightColor;

void main()
{             
    // retrieve existing color in case current pixel was already lit in the previous pass
    vec3 screenColor = texture(screenTexture, TexCoords).rgb;;
    // retrieve data from gbuffer
    vec3 color = texture(gAlbedo, TexCoords).rgb;
    float lightEnabled = texture(gLightEnabled, TexCoords).r;
    float ambientOcclusion = texture(ssao, TexCoords).r;
    
    vec3 finalColor = screenColor;
    if (lightEnabled > 0.5)
    {
        // then calculate lighting as usual
        vec3 lighting  = ambientLightColor * color;
        lighting *= ambientOcclusion;
        finalColor += lighting;
    }
    else
    {
        // in case pixel is unlit, set it to its albedo
        // override because only lighting is additive, not base color
        finalColor = color;
    }

    FragColor = vec4(finalColor, 1.0);
}