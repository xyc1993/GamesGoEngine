#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D gAlbedo;
layout(binding = 2) uniform sampler2D gAmbientOcclusion;
layout(binding = 3) uniform sampler2D gEmissive;
layout(binding = 4) uniform sampler2D ssao;

uniform float ambientLightActive;
uniform vec3 ambientLightColor;

void main()
{             
    // retrieve existing color in case current pixel was already lit in the previous pass
    vec3 screenColor = texture(screenTexture, TexCoords).rgb;
    // retrieve data from gbuffer
    vec3 color = texture(gAlbedo, TexCoords).rgb;
    float ambientOcclusion = texture(gAmbientOcclusion, TexCoords).r;
    vec3 emissive = texture(gEmissive, TexCoords).rgb;    
    float screenSpaceAmbientOcclusion = texture(ssao, TexCoords).r;
    
     // use screen texture in case we're additively combining light
    vec3 finalColor = screenColor;

    // calculate lighting as usual
    vec3 lighting  = ambientLightColor * color;
    lighting *= ambientOcclusion;
    lighting *= screenSpaceAmbientOcclusion;
    finalColor += lighting;

    // lastly add emissive
    finalColor += emissive;

    FragColor = vec4(finalColor, 1.0);
}