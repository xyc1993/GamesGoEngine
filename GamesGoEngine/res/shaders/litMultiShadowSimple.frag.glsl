#version 420 core
out vec4 FragColor;

#define NUMBER_OF_DIR_LIGHTS 4
#define NUMBER_OF_POINT_LIGHTS 4
#define NUMBER_OF_SPOT_LIGHTS 4

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    // used for directional light shadows calculation
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 objectColor;
uniform sampler2D directionalLightShadowMap;
uniform samplerCube pointLightShadowMap;

uniform float far_plane;

layout(std140, binding = 1) uniform CameraData
{
    vec3 cameraPos;
    vec3 cameraDir;
};

float DirectionalLightShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // shadow smoothing via PCF (percentage-closer filtering)
    float shadow = 0.0;
    float bias = 0.005;
    vec2 texelSize = 1.0 / textureSize(directionalLightShadowMap, 0);
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
                float pcfDepth = texture(directionalLightShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                // check whether current frag pos is in shadow
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }      
        }    
    }
    shadow /= 9.0;

    return shadow;
}

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float PointLightShadowCalculation(vec3 fragPos, vec3 lightPos)
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

// ambient light is going to be separate from other light sources
uniform float ambientLightActive = 0.0;
uniform vec3 ambientLightColor;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform int dirLightsNumber;
uniform DirLight dirLight[NUMBER_OF_DIR_LIGHTS];

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
    vec3 color = objectColor;
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(cameraPos - fs_in.FragPos);

    // calculate final color
    vec3 finalColor = vec3(0.0);

    // add ambient light
    vec3 ambient = step(0.5, ambientLightActive) * ambientLightColor;
    finalColor += ambient * color;

    if (dirLightsNumber > 0)
    {
        vec3 lightDir = normalize(-dirLight[0].direction);
	    vec3 halfwayDir = normalize(lightDir + viewDir);

	    float diff = max(dot(normal, lightDir), 0.0);

	    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	    // multiply by diff to ensure specular will be 0 if the pixel is facing light 'backwards'
	    //spec *= step(SPEC_THRESHOLD, diff);

	    vec3 diffuse = dirLight[0].diffuse * diff * color;
	    vec3 specular = dirLight[0].specular * spec * color;

        float shadow = DirectionalLightShadowCalculation(fs_in.FragPosLightSpace);
        vec3 dirLightColor = (1.0 - shadow) * (diffuse + specular); 

        finalColor += dirLightColor;
    }

    // add point light influence
    if (pointLightsNumber > 0)
    {
        // diffuse
        vec3 lightDir = normalize(pointLights[0].position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        // specular        
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);    

        vec3 pointLightColor = vec3(0.0);

        // calculate lighting based on provided light data
	    vec3 diffuse = pointLights[0].diffuse * diff * color;
	    vec3 specular = pointLights[0].specular * spec * color;

        // calculate and apply shadow
        float shadow = PointLightShadowCalculation(fs_in.FragPos, pointLights[0].position);
        pointLightColor = (1.0 - shadow) * (diffuse + specular);

        // limit light effect based on distance
        float distance = length(pointLights[0].position - fs_in.FragPos);
        float attenuation = 1.0 / (pointLights[0].constant + pointLights[0].linear * distance + pointLights[0].quadratic * distance * distance);
        pointLightColor *= attenuation;

        finalColor += pointLightColor;
    }
    
    FragColor = vec4(finalColor, 1.0);
}  