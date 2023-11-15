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

layout (binding = 0) uniform sampler2D albedoMap;
layout (binding = 1) uniform sampler2D normalMap;
layout (binding = 2) uniform sampler2D metallicMap;
layout (binding = 3) uniform sampler2D roughnessMap;
layout (binding = 4) uniform sampler2D aoMap;
layout (binding = 5) uniform sampler2D emissiveMap;

uniform float emissiveStrength;

vec3 getNormalFromMap(vec3 worldPos, vec3 worldNormal)
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(worldPos);
    vec3 Q2 = dFdy(worldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N = normalize(worldNormal);
    vec3 T = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{    
   // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(FragPos, 1.0);
    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(getNormalFromMap(FragPos, normalize(Normal)), 1.0);
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