#version 420 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpecular;
layout (location = 4) out vec4 gLightEnabled;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float enableLight;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(FragPos, 1.0);
    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(normalize(Normal), 1.0);
    // diffuse per-fragment color
    gAlbedo = texture(texture_diffuse1, TexCoords);
    // specular per-fragment color
    gSpecular = texture(texture_specular1, TexCoords);
    // enable light for object
    gLightEnabled = vec4(enableLight, 1.0 - enableLight, 0.0, 1.0);
}