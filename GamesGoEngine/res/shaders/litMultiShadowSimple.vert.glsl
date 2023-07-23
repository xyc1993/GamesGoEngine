#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    // used for shadow mapping
    vec4 FragPosDirectionalLightSpace;
    vec4 FragPosSpotLightSpace;
} vs_out;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;
uniform mat4 directionalLightSpaceMatrix;
uniform mat4 spotLightSpaceMatrix;

void main()
{           
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosDirectionalLightSpace = directionalLightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    vs_out.FragPosSpotLightSpace = spotLightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}