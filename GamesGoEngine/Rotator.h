#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

/*
 * Test class used for testing rotation change
 */
class Rotator : public Component
{
public:
	Rotator();
	
	virtual void Update() override;

	// rotation speed expressed in degrees per second
	void SetSpeed(glm::vec3 speed);

private:
	glm::quat identityQuat;
	glm::quat speed;
};