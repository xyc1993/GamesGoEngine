#version 420 core
layout ( location = 0 ) in vec3 position;

out vec3 TexCoords;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

void main()
{
	// Adjust the view matrix by removing translation from the view matrix
	mat4 skyboxView = view;

	skyboxView[0][3] = 0.0f;
	skyboxView[1][3] = 0.0f;
	skyboxView[2][3] = 0.0f;

	skyboxView[3][3] = 1.0f;

	skyboxView[3][0] = 0.0f;
	skyboxView[3][1] = 0.0f;
	skyboxView[3][2] = 0.0f;

	// Skybox calculation
	vec4 pos = projection * skyboxView * vec4(position, 1.0);
	gl_Position = pos.xyww;
	TexCoords = position;
};