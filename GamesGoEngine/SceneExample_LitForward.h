#pragma once

#include "SceneExample.h"

class SceneExample_LitForward : public SceneExample
{
public:
	SceneExample_LitForward();

	virtual void Update() override;

private:
	static constexpr int LAMPS_NUMBER = 4;

	glm::vec3 startPointLightPositions[LAMPS_NUMBER] = {
		glm::vec3(0.07,   0.2f,   1.0f),
		glm::vec3(2.3f,   -3.3f,   -4.0f),
		glm::vec3(-4.0f,  -2.0f,  -11.0f),
		glm::vec3(0.0f,  0.0f,  -3.0f)
	};

	int lampObjectsIndices[LAMPS_NUMBER];
};