#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D directionalLightShadowMap;
layout(binding = 2) uniform sampler2D gPosition;
layout(binding = 3) uniform sampler2D gNormal;
layout(binding = 4) uniform sampler2D gAlbedo;
layout(binding = 5) uniform sampler2D gSpecular;
layout(binding = 6) uniform sampler2D gLightEnabled;

uniform mat4 directionalLightSpaceMatrix;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform int dirLightsNumber;
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

void main()
{             
    // retrieve existing color in case current pixel was already lit in the previous pass
    vec3 screenColor = texture(screenTexture, TexCoords).rgb;;
    // retrieve data from gbuffer
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 color = texture(gAlbedo, TexCoords).rgb;
    float specular = texture(gSpecular, TexCoords).r;
    float lightEnabled = texture(gLightEnabled, TexCoords).r;
    
    vec3 finalColor = screenColor;
    if (lightEnabled > 0.5)
    {
        // then calculate lighting as usual
        vec3 lighting  = vec3(0.0);
        vec3 viewDir  = normalize(cameraPos - fragPos);

        // diffuse
        vec3 lightDir = normalize(-dirLight.direction);
	    float diff = max(dot(normal, lightDir), 0.0);

        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
	    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	    
        // diffuse and specular colors
	    vec3 diffuse = dirLight.diffuse * diff * color;
	    vec3 specular = dirLight.specular * spec * color;
        lighting = diffuse + specular;

        // shadows
        vec4 fragPosDirectionalLightSpace = directionalLightSpaceMatrix * vec4(fragPos, 1.0);
        float shadow = ShadowCalculation(directionalLightShadowMap, fragPosDirectionalLightSpace, 0.004);
        lighting = (1.0f - shadow) * lighting;

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