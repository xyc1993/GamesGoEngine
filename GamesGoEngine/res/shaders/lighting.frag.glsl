#version 420 core
#define NUMBER_OF_DIR_LIGHTS 4
#define NUMBER_OF_POINT_LIGHTS 4
#define NUMBER_OF_SPOT_LIGHTS 4

#define SPEC_THRESHOLD 0.05

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

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

struct SpotLight
{
	vec3 direction;
	vec3 position;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;

uniform int dirLightsNumber;
uniform int pointLightsNumber;
uniform int spotLightsNumber;

uniform Material material;
uniform DirLight dirLight[NUMBER_OF_DIR_LIGHTS];
uniform SpotLight spotLight[NUMBER_OF_SPOT_LIGHTS];
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0, 0.0 ,0.0);

	//calculate directional light
	for (int i = 0; i < dirLightsNumber; i++)
	{
		result += CalcDirLight(dirLight[i], norm, viewDir);
	}
	
	//calculate point lights
	for (int i = 0; i < pointLightsNumber; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	
	//calculate spotlight
	for (int i = 0; i < spotLightsNumber; i++)
	{
		result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);
	}	

	color = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// multiply by diff to ensure specular will be 0 if the pixel is facing light 'backwards'
	spec *= step(SPEC_THRESHOLD, diff);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// multiply by diff to ensure specular will be 0 if the pixel is facing light 'backwards'
	spec *= step(SPEC_THRESHOLD, diff);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 ambient = light.ambient* vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse* diff* vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular* spec* vec3(texture(material.specular, TexCoord));

	return attenuation * (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// multiply by diff to ensure specular will be 0 if the pixel is facing light 'backwards'
	spec *= step(SPEC_THRESHOLD, diff);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	//spotlight
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	return attenuation * (ambient + intensity * (diffuse + specular));
}