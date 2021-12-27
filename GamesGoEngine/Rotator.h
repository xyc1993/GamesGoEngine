#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

/*
 * Test class used for testing rotation change
 */
class Rotator : public Component
{
public:
	virtual void Update() override;

	void SetSpeed(glm::vec3 speed);

private:
	// rotation speed expressed in degrees per second
	glm::vec3 speed;
};