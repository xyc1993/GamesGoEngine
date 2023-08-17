#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D directionalLightShadowMap;
layout(binding = 2) uniform sampler2D gPosition;
layout(binding = 3) uniform sampler2D gNormal;
layout(binding = 4) uniform sampler2D gAlbedo;
layout(binding = 5) uniform sampler2D gMetallic;
layout(binding = 6) uniform sampler2D gRoughness;
layout(binding = 7) uniform sampler2D gAmbientOcclusion;
layout(binding = 8) uniform sampler2D gEmissive;

uniform mat4 directionalLightSpaceMatrix;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;

layout(std140, binding = 1) uniform CameraData
{
    vec3 cameraPos;
    vec3 cameraDir;
};

// used for directional light and spot lights
float ShadowCalculation(sampler2D sampledTexture, vec4 fragPosLightSpace, float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // shadow smoothing via PCF (percentage-closer filtering)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(sampledTexture, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
        // for objects outside of projection there should be no shadow
            if(projCoords.z > 1.0)
            {
                shadow = 0.0;
            }
            else
            {
                float pcfDepth = texture(sampledTexture, projCoords.xy + vec2(x, y) * texelSize).r;
                // check whether current frag pos is in shadow
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }      
        }    
    }
    shadow /= 9.0;

    return shadow;
}

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
    vec3 worldPos = texture(gPosition, TexCoords).rgb;
    vec3 albedo = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2));
    float metallic = texture(gMetallic, TexCoords).r;
    float roughness = texture(gRoughness, TexCoords).r;    
    vec3 emissive = texture(gEmissive, TexCoords).rgb;

    vec3 N = texture(gNormal, TexCoords).rgb;
    vec3 V = normalize(cameraPos - worldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    // calculate per-light radiance
    vec3 L = normalize(-dirLight.direction);
    vec3 H = normalize(V + L);
    // point light specular is used since it's brightest, relic from older light model
    vec3 radiance = dirLight.specular;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
        
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	  

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);        

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

    vec3 lighting = Lo;

    // calculate and apply shadow
    vec4 fragPosDirectionalLightSpace = directionalLightSpaceMatrix * vec4(worldPos, 1.0);
    float shadow = ShadowCalculation(directionalLightShadowMap, fragPosDirectionalLightSpace, 0.004);
    lighting = (1.0 - shadow) * lighting;

    // emissive
    vec3 color = lighting + emissive;

    // add screen texture in case we're additively combining light
    color += texture(screenTexture, TexCoords).rgb;

    FragColor = vec4(color, 1.0);
}