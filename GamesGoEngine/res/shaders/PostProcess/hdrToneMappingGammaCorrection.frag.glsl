#version 440
out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D depthStencilTexture;
layout(binding = 2) uniform usampler2D stencilView;
layout(binding = 3) uniform sampler2D bloomBlur;

uniform float blendWeight;
uniform float gamma;
uniform float exposure = 1.0f;
uniform float bloomEnabled = 1.0f;

void main()
{
    vec3 hdrColor  = texture(screenTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    vec3 bloom = mix(vec3(0.0f), bloomColor, bloomEnabled);

    // additive blending
    hdrColor += bloom;

    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}