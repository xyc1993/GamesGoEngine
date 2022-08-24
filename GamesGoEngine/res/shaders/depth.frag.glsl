#version 420 core
out vec4 color;

float near = 0.1; 
float far  = 10.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{     
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    color = vec4(vec3(1.0 - depth), 1.0);

    //color = vec4(vec3(1.0 - gl_FragCoord.z), 1.0);
}