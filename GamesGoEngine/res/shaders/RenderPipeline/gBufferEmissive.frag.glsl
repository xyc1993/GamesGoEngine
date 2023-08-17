#version 420 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gMetallic;
layout (location = 4) out vec4 gRoughness;
layout (location = 5) out vec4 gAmbientOcclusion;
layout (location = 6) out vec4 gEmissive;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 emissiveColor;
uniform float emissiveStrength;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(FragPos, 1.0);
    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(normalize(Normal), 1.0);
    // diffuse per-fragment color
    gAlbedo = vec4(vec3(0.0), 1.0);
    // metallic per-fragment color
    gMetallic = vec4(0.0);
    // roughness per-fragment color
    gRoughness = vec4(0.0);
    // ambient occlusion per-fragment color
    gAmbientOcclusion = vec4(vec3(0.0), 1.0);
    // emissive per-fragment color
    gEmissive = vec4(emissiveStrength * emissiveColor, 1.0);
}