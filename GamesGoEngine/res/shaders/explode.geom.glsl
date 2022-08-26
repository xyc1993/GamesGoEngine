#version 420 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT{
    vec2 texCoords;
} gs_in[];

out vec2 TexCoord;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 2) uniform TimeData
{
    float time;
    float deltaTime;
};

vec4 Explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
} 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(b, a));
}  

void main() {
    vec3 normal = GetNormal();

    // projection matrix has to be applied after the vertices's positions are modified by geometry shader
    gl_Position = projection * Explode(gl_in[0].gl_Position, normal);
    TexCoord = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = projection * Explode(gl_in[1].gl_Position, normal);
    TexCoord = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = projection * Explode(gl_in[2].gl_Position, normal);
    TexCoord = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}