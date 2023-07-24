#version 420 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube pointLightShadowMap;

uniform float far_plane;

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

#define NUMBER_OF_POINT_LIGHTS 4

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

uniform int pointLightsNumber;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    
    // calculate final color with provided point light
    vec3 finalColor = vec3(0.0);
    if (pointLightsNumber > 0)
    {
        // diffuse
        vec3 lightDir = normalize(pointLights[0].position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);

        // specular
        vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        // calculate shadow
        float shadow = ShadowCalculation(fs_in.FragPos, pointLights[0].position);

        // calculate lighting based on provided light data
        vec3 ambient = pointLights[0].ambient * color;
	    vec3 diffuse = pointLights[0].diffuse * diff * color;
	    vec3 specular = pointLights[0].specular * spec * color;
        finalColor = ambient + (1.0 - shadow) * (diffuse + specular);

        // limit light effect based on distance
        float distance = length(pointLights[0].position - fs_in.FragPos);
        float attenuation = 1.0 / (pointLights[0].constant + pointLights[0].linear * distance + pointLights[0].quadratic * distance * distance);
        finalColor *= attenuation;
    }
    
    FragColor = vec4(finalColor, 1.0);
}  