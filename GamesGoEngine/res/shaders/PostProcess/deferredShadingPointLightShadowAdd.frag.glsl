#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform samplerCube pointLightShadowMap;
layout(binding = 2) uniform sampler2D gPosition;
layout(binding = 3) uniform sampler2D gNormal;
layout(binding = 4) uniform sampler2D gAlbedo;
layout(binding = 5) uniform sampler2D gSpecular;
layout(binding = 6) uniform sampler2D gLightEnabled;

uniform float far_plane;

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform PointLight pointLight;

layout(std140, binding = 1) uniform CameraData
{
    vec3 cameraPos;
    vec3 cameraDir;
};

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos, vec3 lightPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    // use PCF for smoother shadow edges with less jagged pattern
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(cameraPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(pointLightShadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

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
        vec3 lightDir = normalize(pointLight.position - fragPos);
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * color * pointLight.diffuse;

        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        vec3 specular = pointLight.specular * spec * specular;

        // attenuation
        float distance = length(pointLight.position - fragPos);
        float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;

        // sum of lights
        lighting += diffuse;
        lighting += specular;

        // calculate and apply shadow
        float shadow = ShadowCalculation(fragPos, pointLight.position);
        lighting = (1.0 - shadow) * lighting;

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