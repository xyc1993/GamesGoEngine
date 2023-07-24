#version 420 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D directionalLightShadowMap;

layout(std140, binding = 1) uniform CameraData
{
    vec3 cameraPos;
    vec3 cameraDir;
};

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // shadow smoothing via PCF (percentage-closer filtering)
    float shadow = 0.0;
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

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform int dirLightsNumber;
uniform DirLight dirLight[1]; // only one directional light is supported here

// assume single directional light in scene for this shader
void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);

    vec3 finalColor;
    if (dirLightsNumber > 0)
    {
        // diffuse
        vec3 lightDir = normalize(-dirLight[0].direction);
	    float diff = max(dot(normal, lightDir), 0.0);
        // specular
        vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
	    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	    vec3 diffuse = dirLight[0].diffuse * diff * color;
	    vec3 specular = dirLight[0].specular * spec * color;

        // calculate shadow <- This part works correctly, however there is sth wrong with rest of the shader, lit and unlit areas aren't shaded correctly
        float bias = max(0.005 * (1.0 - dot(normal, dirLight[0].direction)), 0.005);
        float shadow = ShadowCalculation(fs_in.FragPosLightSpace, 0.005);
        vec3 dirLightColor = (1.0 - shadow) * (diffuse + specular); 

        finalColor += dirLightColor;
    }

    FragColor = vec4(finalColor, 1.0);
}