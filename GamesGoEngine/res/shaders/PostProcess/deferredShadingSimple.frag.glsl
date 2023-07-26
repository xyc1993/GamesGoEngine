#version 420 core
#define NUMBER_OF_POINT_LIGHTS 4

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;
layout(binding = 3) uniform sampler2D gSpecular;
layout(binding = 4) uniform sampler2D gLightEnabled;

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

layout(std140, binding = 1) uniform CameraData
{
    vec3 cameraPos;
    vec3 cameraDir;
};

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Color = texture(gAlbedo, TexCoords).rgb;
    float Specular = texture(gSpecular, TexCoords).r;
    float LightEnabled = texture(gLightEnabled, TexCoords).r;
    
    vec3 finalColor = Color;
    if (LightEnabled > 0.5)
    {
        //reset color for light calculation
        finalColor = vec3(0.0);

        // then calculate lighting as usual
        vec3 lighting  = vec3(0.0);
        vec3 viewDir  = normalize(cameraPos - FragPos);

        for(int i = 0; i < pointLightsNumber; ++i)
        {
            // ambient
            vec3 ambient = pointLights[i].ambient * Color;

            // diffuse
            vec3 lightDir = normalize(pointLights[i].position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Color * pointLights[i].diffuse;

            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
            vec3 specular = pointLights[i].specular * spec * Specular;

            // attenuation
            float distance = length(pointLights[i].position - FragPos);
            float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;

            // sum of lights
            lighting += ambient;
            lighting += diffuse;
            lighting += specular;
        }

        finalColor = lighting;
    }

    FragColor = vec4(finalColor, 1.0);
}