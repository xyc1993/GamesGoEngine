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

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D emissiveMap;

uniform float emissiveStrength;

void main()
{    
   // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(FragPos, 1.0);
    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(normalize(Normal), 1.0);
    // diffuse per-fragment color
    gAlbedo = texture(albedoMap, TexCoords);
    // metallic per-fragment color
    gMetallic = texture(metallicMap, TexCoords);
    // roughness per-fragment color
    gRoughness = texture(roughnessMap, TexCoords);
    // ambient occlusion per-fragment color
    gAmbientOcclusion = texture(aoMap, TexCoords);
    // emissive per-fragment color
    gEmissive = vec4(emissiveStrength * vec3(texture(emissiveMap, TexCoords)), 1.0);
}