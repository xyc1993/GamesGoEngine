#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec2 TexCoords;

    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

    vec3 LightPos;
    vec3 ViewPos;
    vec3 FragPos;
} vs_out;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

layout(std140, binding = 1) uniform CameraData
{
    vec3 cameraPos;
    vec3 cameraDir;
};

uniform mat4 model;
uniform vec3 lightPos;

void main()
{
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;
    
    vec3 T   = normalize(mat3(model) * aTangent);
    vec3 B   = normalize(mat3(model) * aBitangent);
    vec3 N   = normalize(mat3(model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * cameraPos;
    vs_out.TangentFragPos  = TBN * fragPos;

    vs_out.LightPos = lightPos;
    vs_out.ViewPos  = cameraPos;
    vs_out.FragPos  = fragPos;
        
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}